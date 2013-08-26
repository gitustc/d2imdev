#include <iostream>
#include<stdio.h>
#include "sprite.hpp"


#define contain(a,b,c) (  (b)<=(c) ? ((a) >= (b) && (a) <= (c)) :  ((a) >= (c) && (a) <= (b)) )
#define debug(info) printf(info)
#define debug1(info,a1) printf(info,a1)
HSL::Image Bore::s_img_bore("./img/bore.bmp");
HSL::Image Bonus::s_img_bonus("./img/bonus.bmp");
HSL::Image Enemy::s_img_enemy("./img/enemy.bmp");
HSL::Image Player::s_img_player0("./img/player1.bmp");
HSL::Image Player::s_img_player1("./img/player2.bmp");
HSL::Image Tank::s_img_shield("./img/shield.bmp");
HSL::Image Bullet::s_img_bullet("./img/bullet.bmp");
HSL::Image Explode::s_img_small_explode("./img/small_explode.bmp");
HSL::Image Explode::s_img_big_explode("./img/big_explode.bmp");
extern int click(int);

//Sprite:
//==========================================
Sprite::Sprite(){
	m_blit_click = 0;
}
Sprite::~Sprite(){}

void Sprite::set_pos(int row,int col){
	m_row = row;
	m_col = col;
}

int Sprite::row()const{
	return m_row;
}

int Sprite::col()const{
	return m_col;
}

int Sprite::hot_row()const{
	return m_hot_row;
}

int Sprite::hot_col()const{
	return m_hot_col;
}

int Sprite::width()const{
	return m_width;
}

int Sprite::height()const{
	return m_height;
}

bool Sprite::check_hit(const Sprite& sp)const{
	int r1,r2,c1,c2;
	int w1,w2,h1,h2;
	r1=m_row-m_hot_row;
	r2=sp.row()-sp.hot_row();
	c1=m_col-m_hot_col;
	c2=sp.col()-sp.hot_col();
	w1=m_width;
	w2=sp.width();
	h1=m_height;
	h2=sp.height();
	return contain( r2,r1-h2,r1+h1) && contain( c2,c1-w2,c1+w1);
}

//Bonus:
//======================================================

Bonus::Bonus(){}
Bonus::~Bonus(){}
bool Bonus::time_out(){
	return m_last_click > click(20*1000);
}


void Bonus::create(BonusType bt,int row,int col){
	m_last_click = 0;
	m_blit_click = 0;
	m_type = bt;
	m_row = row;
	m_col = col;
	m_width = s_img_bonus.width()/6;
	m_height = s_img_bonus.height();
	m_hot_row = m_width/2;
	m_hot_col = m_height /2;
}

BonusType Bonus::type(){
	return m_type;
}



void Bonus::blit(HSL::Image& buffer,int row_off,int col_off){
	m_blit_click ++;
	if((m_blit_click /4)%2 )return;

	buffer.draw(s_img_bonus,
			    0,m_type*m_width,
				m_width,m_height,
				m_row - m_hot_row + row_off,
				m_col - m_hot_col + col_off
				);
}
void Bonus::blit(HSL::Image& buffer,
				 int row_off,int col_off,
				 HSL::Color col){

	m_blit_click ++;
	if((m_blit_click /4)%2 )return;
	buffer.draw(s_img_bonus,
			    0,(int)m_type*m_width,
				m_width,m_height,
				m_row - m_hot_row + row_off,
				m_col - m_hot_col + col_off,
				col
				);
}

void Bonus::update(){
	m_last_click ++;
}



//Explode
//==================================================

Explode::Explode(){}
Explode::~Explode(){}
void Explode::blit(HSL::Image& buffer,int row_off,int col_off){
	//Explode是唯一一个可以越界显示的，故有这一步 
	int br;
	int bc;
	m_blit_click ++;
	br = m_row - m_hot_row + row_off;
	bc = m_col - m_hot_col + col_off;
	switch (m_type){
		case ExplodeTile:
			buffer.draw(s_img_small_explode,
				    0,(m_blit_click%3)*m_width,
					bc >= 0 ? m_width : m_width + bc,
					br >= 0 ? m_height : m_height + br,
					br >= 0 ? br : 0,
					bc >= 0 ? bc : 0
					);
			break;
		case ExplodeTank:
		case ExplodeHawk:
			buffer.draw(s_img_big_explode,
				    0,(m_blit_click%3)*m_width,
					bc >= 0 ? m_width : m_width + bc,
					br >= 0 ? m_height : m_height + br,
					br >= 0 ? br : 0,
					bc >= 0 ? bc : 0
					);
			break;
	}
}
void Explode::blit(HSL::Image& buffer,
		           int row_off,int col_off,
				   HSL::Color co){
	//Explode是唯一一个可以越界显示的，故有这一步 
	int br;
	int bc;
	m_blit_click ++;
	br = m_row - m_hot_row + row_off;
	bc = m_col - m_hot_col + col_off;
	switch (m_type){
		case ExplodeTile:
			buffer.draw(s_img_small_explode,
				    br >= 0 ? 0 : -br,
					(bc >= 0 ? 0 : -bc)+(m_blit_click%3)*m_width,
					bc >= 0 ? m_width : m_width + bc,
					br >= 0 ? m_height : m_height + br,
					br >= 0 ? br : 0,
					bc >= 0 ? bc : 0,
					co
					);
			break;
		case ExplodeTank:
		case ExplodeHawk:
			buffer.draw(s_img_big_explode,
				    0,(m_blit_click%3)*m_width,
					bc >= 0 ? m_width : m_width + bc,
					br >= 0 ? m_height : m_height + br,
					br >= 0 ? br : 0,
					bc >= 0 ? bc : 0,
					co
					);
			break;
	}
}
void Explode::create(ExplodeType et,int row,int col){
	m_type = et;
	m_row = row;
	m_col = col;
	m_last_click = 0;
	m_blit_click = 0;

	switch (m_type){
		case ExplodeTile:
			m_width = 28;
			m_height = 28;
			m_hot_row = 14;
			m_hot_col = 14;
			break;
		case ExplodeTank:
		case ExplodeHawk:
			m_width = 40;
			m_height = 40;
			m_hot_row = 20;
			m_hot_col = 20;
			break;
	}
}



bool Explode::time_out(){
	return m_last_click > click(150);
}
void Explode::update(){
	m_last_click++;
}
//Bore
//==========================================================
Bore::Bore(){
	m_width = 32;
	m_height = 32;
	m_hot_row = 16;
	m_hot_col = 16;
}
Bore::~Bore(){}

bool Bore::time_out(){
	return m_last_click > click(1500);
}


void Bore::blit(HSL::Image& buffer,int row_off,int col_off){
	m_blit_click ++;
	buffer.draw(s_img_bore,
			    0,(m_blit_click%4)*m_width,
				m_width,m_height,
				m_row - m_hot_row + row_off,
				m_col - m_hot_col + col_off
				);
}
void Bore::blit(HSL::Image& buffer,
		             int row_off,int col_off,
					 HSL::Color col){
	m_blit_click ++;
	if(m_blit_click%2 )return;
	buffer.draw(s_img_bore,
			    0,(m_blit_click%4)*m_width,
				m_width,m_height,
				m_row - m_hot_row + row_off,
				m_col - m_hot_col + col_off,
				col
				);
}

void Bore::update(){
	m_last_click ++;
}

void Bore::create(int row,int col){
	m_last_click = 0;
	m_blit_click = 0;
	m_row = row;
	m_col = col;
}


//SpriteMovble
//=============================================================


SpriteMovble::SpriteMovble(){}
SpriteMovble::~SpriteMovble(){}

void SpriteMovble::set_dir(Direction dir){
	m_dir = dir;
}
Direction SpriteMovble::dir(){
	return m_dir;
}
void SpriteMovble::add_pos(int row,int col){
	m_row += row;
	m_col += col;
}
void SpriteMovble::add_row(int pix){
	m_row += pix;
}

void SpriteMovble::add_col(int pix){
	m_col += pix;
}

void SpriteMovble::move(){
	switch (m_dir){
		case DirUp:
			add_row(-1*speed());
			break;
		case DirDown:
			add_row(speed());
			break;
		case DirLeft:
			add_col(-1*speed());
			break;
		case DirRight:
			add_col(speed());
			break;
		default:
			break;
	}
}
//Bullet
//===============================================



Bullet::Bullet(){
	m_width = 8;
	m_height = 8;
	m_hot_row = 4;
	m_hot_col = 4;
}

Bullet::~Bullet(){

}
BulletLevel Bullet::level(){
	return m_level;
}

void Bullet::create(BulletLevel bl,
		 Direction dir,
		 int row, int col){

	m_level = bl;
	m_row = row;
	m_col = col;
	m_dir = dir;
//	m_blit_click = 0;     //没需要
}

void Bullet::add_level(int bl){
	m_level = (BulletLevel)(m_level + bl);
}

void Bullet::blit(HSL::Image& buffer,
		          int row_off,int col_off,
				  HSL::Color co){
	buffer.draw(s_img_bullet,
			    0,(int)m_dir*8,
				8,8,
				m_row - m_hot_row + row_off,
				m_col - m_hot_col + col_off,
				co);

}
void Bullet::blit(HSL::Image& buffer,int row_off,int col_off){
	buffer.draw(s_img_bullet,
			    0,(int)m_dir*8,
				8,8,
				m_row - m_hot_row + row_off,
				m_col - m_hot_col + col_off);

}

void Bullet::update(){
//安照规矩,这里的update不会更新坐标
//但是又没其他的好更新的,故为空

	fprintf(stderr,"update will not change the offset !!\n");
}

int Bullet::speed(){
	return (1.+(int)m_level*.2)*6;
}

//Tank
//==================================================================

Tank::Tank(){
	m_width = 28;
	m_height = 28;
	m_hot_row = 14;
	m_hot_col = 14;
	m_level = TankLevel0;
	m_life_left = 1;
}

Tank::~Tank(){}

void Tank::set_shield(bool fl){
	m_shield = fl;
	m_shield_click = 0;
}

bool Tank::shield(){
	return m_shield ;
}
int Tank::speed(){
	return (1.+(int)m_level*0.22)*2.0; 
}

TankLevel Tank::level(){
	return m_level;
}

void Tank::add_life(int add){
	m_life_left += add;
}

int Tank::life(){
	return m_life_left;
}

void Tank::add_level(int le){
	int ll = (int)m_level + le;
	if(ll >= 3){
		m_level = TankLevel3;
	}else{
		m_level = (TankLevel)ll;
	}
}


//Player
//=========================================================
Player::Player():Tank(){}
Player::~Player(){}
void Player::blit(HSL::Image& buffer,int row_off,int col_off){
	if(m_state == PlayerWink){
		if((m_wink_click/3)%3)return;
	}
	m_blit_click ++ ;
	if(m_type == PlayerTypeOne){
		buffer.draw(s_img_player0,
				    (int)m_dir*28,((m_blit_click/2)%2 + 2*(int)m_level)*28,
					28,28,
					m_row - m_hot_row + row_off,
					m_col - m_hot_col + col_off);
		if(m_shield){
			buffer.draw(s_img_shield,
					32*((m_blit_click/2)%2),0,
					32,32,
					m_row - m_hot_row - 4 + row_off + 2,
					m_col - m_hot_col - 4 + col_off + 2,//加2为微调，无理论关系

					RGB(0,0,0)
					);
		}
	}else{
		buffer.draw(s_img_player1,
				    (int)m_dir*28,((m_blit_click/2)%2 + 2*(int)m_level)*28,
					28,28,
					m_row - m_hot_row + row_off,
					m_col - m_hot_col + col_off);
		if(m_shield){
			buffer.draw(s_img_shield,
					32*((m_blit_click/2)%2),0,
					32,32,
					m_row - m_hot_row - 4 + row_off + 2,
					m_col - m_hot_col - 4 + col_off + 2, //加2为微调，无理论关系
					RGB(0,0,0)
					);
		}
	}
}


void Player::blit(HSL::Image& buffer,int row_off,int col_off,HSL::Color){
	//通常不使用,因为Player为精灵层,不需Mask
	fprintf(stderr,"On defined player blit with mask ...\n");
}

void Player::create(PlayerType pt,int row ,int col){
	m_type = pt;
	m_row = row;
	m_col = col;
	m_dir = DirUp; //默认为向上,和游戏有关 合理吗....
	m_level = TankLevel0;
	//m_level = TankLevel3;
	m_life_left = 1;
	m_state = PlayerNormal;
	m_blit_click = 0;
}
void Player::set_state(PlayerState ps){
	m_state = ps;
	m_wink_click = 0;
}

PlayerState Player::state(){
	return m_state;
}
void Player::update(){
	if(m_shield){
		m_shield_click ++;
		if(m_shield_click > click(8000)){
			m_shield = false;
		}
	}
	if(m_state == PlayerWink){
		m_wink_click ++;
		if(m_wink_click > click(8000)){
			m_state = PlayerNormal;
		}
	}
}

bool Player::hitted(){
	if(m_shield)return false;
	return true;
};


//Enemy
//============================================================

Enemy::Enemy():Tank(){}
Enemy::~Enemy(){}

EnemyType Enemy::type(){
	return m_type;
}

void Enemy::create(EnemyType et,bool flash,int row,int col){

	m_type = et;
	m_row = row;
	m_col = col;
	m_flash = flash;
	m_dir = DirDown;
//	m_shield = true;
	m_last_attack_click = rand()%(click(2*1000));
	m_blit_click = 0;

	if( et == EnemyTypeThree)
		m_life_left = 3;
	else
		m_life_left = 1;
}


void Enemy::update(){
	if(m_shield){
		m_shield_click ++;
		if(m_shield_click >= click(8000)){
			m_shield = false;
		}
	}
	m_last_attack_click ++;

}

void Enemy::set_attack_click(int click){
	m_last_attack_click = click;
}

int Enemy::attack_click(){
	return m_last_attack_click;
}

bool Enemy::flash(){
	return m_flash;
}


void Enemy::blit(HSL::Image& buffer,int row_off,int col_off){

	m_blit_click ++;

	switch(m_type){
		case EnemyTypeOne:
			if(m_flash){
				buffer.draw(s_img_enemy,
						    (int)m_dir*28,((m_blit_click/2)%4)*28, //0 0 1 1 2 2 3 3 0 0 1 1 ....
							28,28,
							m_row - m_hot_row + row_off,
							m_col - m_hot_col + col_off);
			}else{
				buffer.draw(s_img_enemy,
						    (int)m_dir*28,((m_blit_click/2)%2)*28, // 0 0 1 1 0 0 1 1 
							28,28,
							m_row - m_hot_row + row_off,
							m_col - m_hot_col + col_off);

			}
			break;

		case EnemyTypeTwo:
			if(m_flash){
				buffer.draw(s_img_enemy,
						    (int)m_dir*28,(((m_blit_click/2)%4) + 4)*28,
							28,28,
							m_row - m_hot_row + row_off,
							m_col - m_hot_col + col_off);
			}else{
				buffer.draw(s_img_enemy,
						    (int)m_dir*28,(((m_blit_click/2)%2) + 4)*28,
							28,28,
							m_row - m_hot_row + row_off,
							m_col - m_hot_col + col_off);

			}
			break;
	//	case EnemyTypeThree:
		default:
			if(m_flash){
				buffer.draw(s_img_enemy,
						    ((int)m_dir+4)*28,(((m_blit_click/2)%2) + ((m_blit_click/4)%2)*6)*28,
							28,28,
							m_row - m_hot_row + row_off,
							m_col - m_hot_col + col_off);
			}else{
				buffer.draw(s_img_enemy,
						    ((int)m_dir+4)*28,((m_blit_click/2)%2+(3-m_life_left)*2)*28 ,
							28,28,
							m_row - m_hot_row + row_off,
							m_col - m_hot_col + col_off);
			}
			break;

	}

	if(m_shield){
			buffer.draw(s_img_shield,
					32*((m_shield_click/2)%2),0,
					32,32,
					m_row - m_hot_row - 4 + row_off + 2,
					m_col - m_hot_col - 4 + col_off + 2, //加2为微调，无理论关系
					RGB(0,0,0)
					);
	}

}

void Enemy::blit(HSL::Image& buffer,int row_off,int col_off,HSL::Color){
	fprintf(stderr,"No defination of enemy blit with mask...\n");
}


bool Enemy::hitted(){
	//
	if(m_flash){
		m_flash = false;
		return false;
	}
	if(m_shield){
		return false;
	}
	m_life_left -- ;
	if(m_life_left == 0)
		return true;
	return false;
}





