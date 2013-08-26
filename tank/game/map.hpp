#ifndef map_h
#define map_h

#include "hsl/hsl.hpp"
typedef enum {
	BlockNull = 0,
	BlockBrick,    //砖
	BlockConcrete, //钢板
	BlockTree,
	BlockRiver1,
	BlockRiver2,
	BlockHawk,
	BlockStone,    //死掉的老王
	BlockMix,		//杂和板
}BlockType;



class Tile{
	public:
		BlockType type();
		BlockType type(int);
		void set_type(BlockType);
		void set_type(int,BlockType);
		private:
		BlockType m_data[5];
};



class Map {
	public:
		Map();
		virtual ~Map();


	public:
		bool  load(const char*);


		Tile& tile(int,int);
		Tile& pixel_tile(int,int);


		// 当前点的绝对属性
		BlockType  pixel_type(int,int);

		void blit_tile (HSL::Image&,int,int);
		void blit_floor(HSL::Image&,int,int);
		void blit_cover(HSL::Image&,int,int);


	public:

		void    set_tile(int,int,BlockType);
		void    set_tile(int,int,int,BlockType);

		void    set_hawk_protected();
		void	set_hawk_bare();//老王被脱光
		void    set_hawk_normal();
		void    update();
		void    update_river();

		int 	pixel_width();
		int 	pixel_height();
		int 	tile_width();
		int 	tile_height();





	private:
		#define m_width 13
		#define m_height 13
		bool 	m_protected;
		bool    m_bare;
		int		m_protect_click;
		int 	m_bare_click;
		Tile	m_tile_array[m_height*m_width];
		static  HSL::Image s_img_tile;
	
};


#endif
