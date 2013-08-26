#include <stdio.h>
#include "map.hpp"

#define debug(info) 
#define debug1(info,a1) 
#define debug2(info,a1,a2) 
#define debug3(info,a1,a2,a3) 
#define debug4(info,a1,a2,a3,a4) 
#define debug5(info,a1,a2,a3,a4,a5) 




//#define debug(info) printf(info)
//#define debug1(info,a1) printf(info,a1)
//#define debug2(info,a1,a2) printf(info,a1,a2)
//#define debug3(info,a1,a2,a3) printf(info,a1,a2,a3)
//#define debug4(info,a1,a2,a3,a4) printf(info,a1,a2,a3,a4)
//#define debug5(info,a1,a2,a3,a4,a5) printf(info,a1,a2,a3,a4,a5)
//

#define tile_block_index(row,col) ( ((row)%32 <16 ? 0:2 ) + ((col)%32 < 16 ? 0:1 ) )
extern int click(int);
HSL::Image Map::s_img_tile("./img/tile.bmp");

//Tile============================================================
void Tile::set_type(BlockType bt){
	m_data[0]=bt;
}

void Tile::set_type(int i,BlockType bt){
	m_data[i+1] = bt;
}

BlockType Tile::type(){
	return m_data[0];
}

BlockType Tile::type(int i){
	return m_data[i+1];
}

//Map===================================================================


Map::Map(){}

Map::~Map(){}


bool Map::load(const char* path){
	char ch;
	int  cou = 0;

	debug1("load %s \n",path);
	FILE* fp = fopen(path,"rb");
	if (!fp){
		fprintf(stderr,"fail to load file: %s \n",path);
		return false;
	}

	while( cou < 13*13 ){
		ch = fgetc(fp);
		if( ch >='0' && ch <= '8' ){
			//得到地图描述符
			//debug1("%c\n",ch);
			if(ch != '8'){
				//非混合块:最高层
				m_tile_array[cou].set_type(  (BlockType)( ch - '0'));
				m_tile_array[cou].set_type(0,(BlockType)( ch - '0'));
				m_tile_array[cou].set_type(1,(BlockType)( ch - '0'));
				m_tile_array[cou].set_type(2,(BlockType)( ch - '0'));
				m_tile_array[cou].set_type(3,(BlockType)( ch - '0'));
				debug3("第%d行第%d列为:单一块,类型为%c\n",cou/13,cou%13,ch);
			}else{
				//混合块 先填写最高层块
				debug2("第%d行第%d列为:合成块,以下为其结构\n",cou/13,cou%13);
				m_tile_array[cou].set_type(BlockMix);

				//填写四个小块:次层
				for ( int i =0;i <4;i++){
					ch = fgetc(fp);
					if(ch >= '0' && ch <= '7'){//仅支持2*2,这里不再有8,若有 忽略
						m_tile_array[cou].set_type(i,(BlockType)(ch-'0'));
					}else i--;
				}
			}
		cou ++;
		}
	}
	while( (ch=fgetc(fp)) != EOF){
		if(ch <='8' && ch >='0')
			goto end;
	}

	fclose(fp);
	debug1("succeed in loading %s\n",path);
	return true;

	end:
	fclose(fp);
	fprintf(stderr,"incorrect map : %s\n",path);
	return false;
}



Tile& Map::tile(int row,int col){
	return m_tile_array[row*m_width+col];
}

void Map::blit_floor(HSL::Image& dst,int row_off,int col_off){

	int src_row_i;
	int src_col_i;
	int dst_row,dst_col;

	for(int row=0;row<13;row++)
		for(int col=0;col<13;col++){


			Tile* tmp = &tile(row,col);
			dst_row = row_off + 32*row;
			dst_col = col_off + 32*col;

			if(tmp->type() != BlockMix ){
				if(tmp->type() != BlockTree){
					
					dst.draw(s_img_tile,
							 0,tmp->type()*32, 
							 32,32,
							 dst_row,dst_col);
				}
			}else {
				for(int i=0;i<4;i++){
		
					src_row_i = (i/2) * 16;
					src_col_i = (i%2) * 16;
		
					if(tmp->type(i) != BlockTree){
						dst.draw(s_img_tile,
								src_row_i,src_col_i+tmp->type(i)*32,
								16,16,
								dst_row+src_row_i,dst_col+src_col_i);
					}
				}
			}
		}
}


void Map::update_river(){
	for(int row=0;row<13;row++)
		for(int col=0;col<13;col++){
			Tile* tmp = &tile(row,col);

			if(tmp->type() != BlockMix ){
				if(tmp->type() == BlockRiver1){
					tmp->set_type(BlockRiver2);
				}else if(tmp->type() == BlockRiver2){
					tmp->set_type(BlockRiver1);
				}
			}else {
				for(int i=0;i<4;i++){
					if(tmp->type(i) == BlockRiver1){
						tmp->set_type(i,BlockRiver2);
					}else if(tmp->type(i) == BlockRiver2){
						tmp->set_type(i,BlockRiver1);
					}
				}
			}
		}
}



void Map::blit_cover(HSL::Image& dst,int row_off,int col_off){

	int src_row_i;
	int src_col_i;
	int dst_row,dst_col;

	for(int row=0;row<13;row++)
		for(int col=0;col<13;col++){


			Tile* tmp = &tile(row,col);
			dst_row = row_off + 32*row;
			dst_col = col_off + 32*col;

			if(tmp->type() != BlockMix ){
				//纯
				if(tmp->type() == BlockTree){
					dst.draw(s_img_tile,
							 0,(int)BlockTree*32, 
							 32,32,
							 dst_row,dst_col,
							 RGB(0,0,0));
				}
			}else {
				//混合
				for(int i=0;i<4;i++){
		
					src_row_i = (i/2) * 16;
					src_col_i = (i%2) * 16;
		
					if(tmp->type(i) == BlockTree){
						dst.draw(s_img_tile,
								 src_row_i,src_col_i+(int)BlockTree*32,
								 16,16,
								 dst_row+src_row_i,dst_col+src_col_i,
								 RGB(0,0,0));
					}
				}
			}
		}
}





void Map::blit_tile(HSL::Image& dst,int row_off,int col_off)
{
	debug2("draw row=%d,col=%d.\n",row,col);
	int src_row_i;
	int src_col_i;
	int dst_row,dst_col;

	for(int row=0;row<13;row++)
		for(int col=0;col<13;col++){

			Tile* tmp = &tile(row,col);
			dst_row = row_off + 32*row;
			dst_col = col_off + 32*col;

			if(tmp->type() != BlockMix){
				debug3("显示:第%d行,第%d列为单一块,类型为:%d\n",row,col,(int)tmp->type());
				dst.draw(s_img_tile,
						 0,tmp->type()*32, 
						 32,32,
						 dst_row,dst_col);
			} else {
				debug2("显示:第%d行,第%d列为合成块,以下为其结构:\n",row,col);
				for(int i=0;i<4;i++){
		
		//		0->(0 0)
		//		1->(0,16)
		//		2->(16,0)
		//		3->(16,16)
					src_row_i = (i/2) * 16;
					src_col_i = (i%2) * 16;
		
					debug4("显示:第%d行,第%d列,第%d子块为单一块,类型为:%d\n",row,col,i,(int)tmp->type(i));
					dst.draw(s_img_tile,
							 src_row_i,src_col_i+tmp->type(i)*32,
							 16,16,	
							 dst_row+src_row_i,dst_col+src_col_i);
				}
			}
		}
}


tile* map_cood_to_tile( map* p, int cood_row, int cood_col ){
    return p->tile + ( cood_row/32*MAP_WIDTH + cood_col/32 );
}

int map_cood_to_tile( map* p, int cood_row, int cood_col ){
    return 

BlockType Map::pixel_type(int p_row,int p_col){

	return pixel_tile(p_row,p_col).type(tile_block_index(p_row,p_col));
}

void Map::set_tile(int row,int col,BlockType ty){
	tile(row,col).set_type(ty);
}
void Map::set_tile(int row,int col,int i,BlockType ty){
	tile(row,col).set_type(i,ty);
}


int Map::pixel_width(){
	return 32*13;
}
int Map::pixel_height(){
	return 32*13;
}

int Map::tile_width(){
	return 13;
}
int Map::tile_height(){
	return 13;
}


void Map::update(){

	static int cou=0;
	if(m_protected){
		m_protect_click ++;
		if(m_protect_click > click(40*1000)){
			set_hawk_normal();
		}

	}
	if(m_bare){
		m_bare_click ++;
		if(m_bare_click > click(40*1000)){
			set_hawk_normal();
		}

	}
	cou ++;
	if(cou%10 == 0)update_river();
}

void Map::set_hawk_protected(){
	if(m_bare)
		set_hawk_normal();
	else{
		m_protected = true;
		m_protect_click = 0;

		tile(11,5).set_type(BlockMix);
		tile(11,5).set_type(0,BlockNull);
		tile(11,5).set_type(1,BlockNull);
		tile(11,5).set_type(2,BlockNull);
		tile(11,5).set_type(3,BlockConcrete);
	
	
		tile(11,6).set_type(BlockMix);
		tile(11,6).set_type(0,BlockNull);
		tile(11,6).set_type(1,BlockNull);
		tile(11,6).set_type(2,BlockConcrete);
		tile(11,6).set_type(3,BlockConcrete);
	
	
		tile(11,7).set_type(BlockMix);
		tile(11,7).set_type(0,BlockNull);
		tile(11,7).set_type(1,BlockNull);
		tile(11,7).set_type(2,BlockConcrete);
		tile(11,7).set_type(3,BlockNull);
	
		tile(12,5).set_type(BlockMix);
		tile(12,5).set_type(0,BlockNull);
		tile(12,5).set_type(1,BlockConcrete);
		tile(12,5).set_type(2,BlockNull);
		tile(12,5).set_type(3,BlockConcrete);
	
	
		tile(12,7).set_type(BlockMix);
		tile(12,7).set_type(0,BlockConcrete);
		tile(12,7).set_type(1,BlockNull);
		tile(12,7).set_type(2,BlockConcrete);
		tile(12,7).set_type(3,BlockNull);
	
	}
}


void Map::set_hawk_bare(){

	if(m_protected)
		set_hawk_normal();
	else{
		m_bare = true;
		m_bare_click = 0;

		tile(11,5).set_type(BlockNull);
		tile(11,6).set_type(BlockNull);
		tile(11,7).set_type(BlockNull);
		tile(12,5).set_type(BlockNull);
		tile(12,7).set_type(BlockNull);
	}

}

void Map::set_hawk_normal(){
	m_protected = false;
	m_bare = false;

	tile(11,5).set_type(BlockMix);
	tile(11,5).set_type(0,BlockNull);
	tile(11,5).set_type(1,BlockNull);
	tile(11,5).set_type(2,BlockNull);
	tile(11,5).set_type(3,BlockBrick);


	tile(11,6).set_type(BlockMix);
	tile(11,6).set_type(0,BlockNull);
	tile(11,6).set_type(1,BlockNull);
	tile(11,6).set_type(2,BlockBrick);
	tile(11,6).set_type(3,BlockBrick);


	tile(11,7).set_type(BlockMix);
	tile(11,7).set_type(0,BlockNull);
	tile(11,7).set_type(1,BlockNull);
	tile(11,7).set_type(2,BlockBrick);
	tile(11,7).set_type(3,BlockNull);

	tile(12,5).set_type(BlockMix);
	tile(12,5).set_type(0,BlockNull);
	tile(12,5).set_type(1,BlockBrick);
	tile(12,5).set_type(2,BlockNull);
	tile(12,5).set_type(3,BlockBrick);


	tile(12,7).set_type(BlockMix);
	tile(12,7).set_type(0,BlockBrick);
	tile(12,7).set_type(1,BlockNull);
	tile(12,7).set_type(2,BlockBrick);
	tile(12,7).set_type(3,BlockNull);
}
