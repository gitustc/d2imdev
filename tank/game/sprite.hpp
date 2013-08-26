#ifndef sprite_h
#define sprite_h
#include "hsl/hsl.hpp"

typedef enum {
	BonusLife=0,
	BonusClock,
	BonusShovel, //铲子
	BonusBomb,
	BonusStar,
	BonusHelmet,  //安全帽
}BonusType;


typedef enum {
	DirUp=0,
	DirRight,
	DirDown,
	DirLeft,
}Direction;


typedef enum {
	ExplodeTank = 0,
	ExplodeTile,
	ExplodeHawk,
}ExplodeType;

typedef enum {
	BulletLevel0=0,
	BulletLevel1,
	BulletLevel2,
	BulletLevel3,
}BulletLevel;

typedef enum {
	TankLevel0 = 0,
	TankLevel1,
	TankLevel2,
	TankLevel3,
}TankLevel;

typedef enum {
	PlayerNormal=0,
	PlayerWink,
}PlayerState;

typedef enum {
	EnemyTypeOne=0,
	EnemyTypeTwo,
	EnemyTypeThree,
}EnemyType;

typedef enum {
	PlayerTypeOne=0,
	PlayerTypeTwo,
}PlayerType;

typedef struct {
	int x,y;
}Position;

class Sprite {
	protected:
		Sprite();
		virtual ~Sprite();
	public:
		virtual void blit(HSL::Image&,int,int) = 0;
		virtual void blit(HSL::Image&,int,int,HSL::Color) = 0;
		virtual void update() = 0;
	public:
		void set_pos(int,int);
		int  row()const;
		int  col()const;
		int  hot_row()const;
		int  hot_col()const;
		int  width()const;
		int  height()const;
		bool check_hit(const Sprite&)const;
	public:
		int m_row,m_col;
		int m_hot_row;
		int m_hot_col;
		int m_width;
		int m_height;
		int m_blit_click;
};



class Bonus : public Sprite {
	public:
		Bonus();
		virtual ~Bonus();
	public:
		bool time_out();
		void create(BonusType,int,int);
		BonusType type();
		void blit(HSL::Image&,int,int,HSL::Color);
		void blit(HSL::Image&,int,int);
		void update();
	private:
		static HSL::Image s_img_bonus;
	private:
		BonusType m_type;
		int m_last_click;
};

class Explode : public Sprite {
	public:
		Explode();
		virtual ~Explode();

	public:
		bool time_out();
		void blit(HSL::Image&,int,int,HSL::Color);
		void blit(HSL::Image&,int,int);
		void create(ExplodeType,int,int);
		void update();
		
	private:
		int m_last_click;
		ExplodeType  m_type;

		static HSL::Image s_img_small_explode;
		static HSL::Image s_img_big_explode;
};


class Bore : public Sprite{
	public:
		Bore();
		virtual ~Bore();

	public:
		bool time_out();
		void create(int,int);
		void blit(HSL::Image&,int,int);
		void blit(HSL::Image&,int,int,HSL::Color);
		void update();

	protected:
		int m_last_click;
		static HSL::Image s_img_bore;

};


class SpriteMovble : public Sprite {

	protected:
		SpriteMovble();
		virtual ~SpriteMovble();

	public:
		void set_dir(Direction);
		Direction dir();
		void add_pos(int,int);
		void add_row(int);
		void add_col(int);
		void move();

		//所有的可移动精灵的update都不会包含对坐标的变化
		virtual void update()=0;
		virtual int  speed() = 0;
	protected:
		Direction m_dir;
		

};
class Bullet : public SpriteMovble {
	public:
		Bullet();
		virtual ~Bullet();

	public:
		BulletLevel level();
		void        create(BulletLevel,Direction,int,int);
		void 		blit(HSL::Image&,int,int,HSL::Color);
		void 		blit(HSL::Image&,int,int);
		void 		add_level(int);
		void update();
		int speed();

	private:
		BulletLevel m_level;
		

	private:
		static HSL::Image s_img_bullet;
};


class Tank : public SpriteMovble {

	public:
		Tank();
		virtual ~Tank();
	public:

		int  speed();

		void set_shield(bool);
		bool shield();

		TankLevel  level();
		void add_level(int);

		int life();
		void add_life(int);

		virtual void update() = 0;
		virtual bool hitted() = 0;


	protected:
		TankLevel m_level;
		bool      m_shield;
		int 	  m_shield_click;
		int 	  m_life_left;
		static HSL::Image s_img_shield;


};

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
