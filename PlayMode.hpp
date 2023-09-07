#include "PPU466.hpp"
#include "Mode.hpp"
#include "load_save_png.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct Bullet{
	bool isActive=false;
	glm::vec2 bullet_at=glm::vec2(0.0f);
	int BulletDirection=0;//1 left 2 right 3 up 4 down
	Bullet();
	~Bullet();
	void Init(glm::vec2 startPos,int direction){
		isActive=true;
		bullet_at=startPos;
		BulletDirection=direction;
	};


};
struct Enemy{
	bool isActive=true;
	glm::vec2 enemy_at=glm::vec2(0.0f);
	int EnemyDirection=1;
};

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;
	void emitBullet(glm::vec2 pos,int direction);
	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up, space;

	//some weird background animation:
	float background_fade = 0.0f;

	//player position:
	glm::vec2 player_at = glm::vec2(100.0f,100.0f);
	int playerDirection=1;
	std::array< Bullet, 8 > bullet;
	std::array<Enemy,8> enemy;
	//glm::vec2 bullet_at = glm::vec2(0.0f);
	//----- drawing handled by PPU466 -----

	PPU466 ppu;
};
