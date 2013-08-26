#ifndef sprite_h
#define sprite_h



#define BONUS_LIFE	0
#define BONUS_CLOCK	1
#define BONUS_SHOVEL	2
#define BONUS_BOMB	3
#define BONUS_STAR	4
#define BONUS_HELMET	5


#define DIR_UP		0
#define DIR_RIGHT	1
#define DIR_DOWN	2
#define DIR_LEFT	3


#define EXPLODE_TANK	0
#define EXPLODE_TILE	1
#define EXPLODE_HAWK	2

#define PLAYER_NORMAL	0
#define PLAYER_WINK	1

typedef void (*blit_func)(sprite*, int, int);
typedef void (*update_func)(sprite*);

typedef struct _sprite{
    sprite* self;
    int     click;
	int		row;
	int		col;
	int		hot_row;
	int		hot_col;
	int		width;
	int		height;
	blit_func	blit;
	update_func	update;
}sprite;

void	sprite_init( sprite* );
int	check_hit( sprite*, sprite* );


#if 0
===================================static_sprite===========================
#endif

typedef struct _static_sprite {
	sprite*		parent;
}static_sprite;

bitmap*		s_bonus_bitmap;
#define		BONUS_MAX_MS		10*1000
typedef struct bonus {
	static_sprite	parent;
	int		click;
}static_sprite;

void		bonus_init( bonus* );




/****************************************************/
bitmap*		s_bore_image;
typedef struct bore {
	static_sprite	parent;
	int		click;
}static_sprite;

#define		bore_image()		s_bore_image;
#define		bore_time_out( p )	((p)->clicks > BORE_MAX_MS*GAME_FPS/1000)
void		bore_init( bore* );


/****************************************************/
typedef struct _movable_sprite {
	sprite		parent;
	int		speed;
	int		dir;
}movable_sprite;


/****************************************************/
typedef struct _bullet {
	movable_sprite	parent;
	int		level;
}bullet;

bitmap*		s_bullet_image;
void bullet_init( bullet* );
#define bullet_image()	s_bullet_image;

/*****************************************************/
bitmap*		s_tank_shield_image;
typedef int (*hitted_func)( tank* );
typedef struct _tank{
	movable_sprite		parent;
	hitted_func		hitted;
	int			shield;
	int			life_left;
}tank;

void tank_init( tank* );
#define tank_shield_image()	s_tank_shield_image;


/*****************************************************/
bitmap*		s_player_shield_image[2];
typedef int (*hitted_func)( tank* );
typedef struct _player{
	tank			parent;
	hitted_func		hitted;
	int			shield;
	int			life_left;
	int			type;
	int			state;
}tank;

void player_init( player* );
#define tank_shield_image( i )	s_tank_shield_image[i];

/*****************************************************************/

bitmap*		s_enemy_image;
typedef struct _enemy{
	tank			parent;
	hitted_func		hitted;
	int			shield;
	int			type;
	int			state;
	int			flash;
	int			attack_click;

}enemy;

void enemy_init( tank* );
#define tank_shield_image( i )	s_tank_shield_image[i];

/*****************************************************************/
class Player : public Tank {
	public:
		Player();
		virtual ~Player();

	public:
		void  create(PlayerType,int,int);
		void  update();
		bool  hitted();

		void set_state(PlayerState);
		PlayerState state();

		void blit(HSL::Image&,int,int,HSL::Color);
		void blit(HSL::Image&,int,int);

	private:
		static HSL::Image s_img_player0;
		static HSL::Image s_img_player1;
		PlayerType  m_type;

		PlayerState m_state;
		int			m_wink_click;
};

class Enemy : public Tank {
	public:
		Enemy();
		virtual ~Enemy();
	public:
		EnemyType type();
		bool flash();

		void create(EnemyType,bool,int,int);
		void update();
		bool hitted();
		void  set_attack_click(int);
		int   attack_click();


		void blit(HSL::Image&,int,int,HSL::Color);
		void blit(HSL::Image&,int,int);

	private:
		EnemyType  m_type;
		bool	   m_flash;
		int		   m_flash_click;
		int		   m_last_attack_click;
		static HSL::Image s_img_enemy;

};


#endif
