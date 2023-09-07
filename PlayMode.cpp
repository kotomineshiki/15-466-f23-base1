#include "PlayMode.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>

#include <random>
std::vector< glm::u8vec4 > GetSprite(std::string filename,glm::uvec2* size){
	std::vector< glm::u8vec4 > data;
	//glm::uvec2* size_p=&size;
	glm::uvec2* size_p=new glm::uvec2(8,8);
    load_png(filename, size_p ,&data , LowerLeftOrigin);
	return data;
}
std::array< glm::u8vec4, 4> GetPal(std::string filename){
	std::vector< glm::u8vec4 > pal;
	glm::uvec2* size=new glm::uvec2(2,2);
    load_png(filename, size ,&pal , LowerLeftOrigin);
	std::array< glm::u8vec4, 4> MyPal=
	{
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
	};
	for(int i=0;i<pal.size();++i){
		MyPal[i]=pal[i];
	}
	return MyPal;
}

void CalculateBit(std::vector< glm::u8vec4 >data,std::array< glm::u8vec4, 4> MyPal, PPU466::Tile* tile){
		for(int i=0;i<8;++i){
		uint8_t bit0=0;
		uint8_t bit1=0;
		for(int j=0;j<8;++j){
			uint8_t toadd0=0;
			uint8_t toadd1=0;
			if(data[8*i+j]==MyPal[0])
			{
				toadd0=0;
				toadd1=0;
			}
			if(data[8*i+j]==MyPal[1])
			{
				toadd0=0;
				toadd1=1;
			}
			if(data[8*i+j]==MyPal[2])
			{
				toadd0=1;
				toadd1=0;
			}
			if(data[8*i+j]==MyPal[3])
			{
				toadd0=1;
				toadd1=1;
			}
			toadd0=toadd0<<j;
			toadd1=toadd1<<j;
			bit0=bit0|toadd0;
			bit1=bit1|toadd1;
		}
		tile->bit0[i]=bit1;
		tile->bit1[i]=bit0;
	}
}
PlayMode::PlayMode() {
	//TODO:
	// you *must* use an asset pipeline of some sort to generate tiles.
	// don't hardcode them like this!
	// or, at least, if you do hardcode them like this,
	//  make yourself a script that spits out the code that you paste in here
	//   and check that script into your repository.

	//Also, *don't* use these tiles in your game:

	{ //use tiles 0-16 as some weird dot pattern thing:
		std::array< uint8_t, 8*8 > distance;
		for (uint32_t y = 0; y < 8; ++y) {
			for (uint32_t x = 0; x < 8; ++x) {
				float d = glm::length(glm::vec2((x + 0.5f) - 4.0f, (y + 0.5f) - 4.0f));
				d /= glm::length(glm::vec2(4.0f, 4.0f));
				distance[x+8*y] = uint8_t(std::max(0,std::min(255,int32_t( 255.0f * d ))));
			}
		}
		for (uint32_t index = 0; index < 16; ++index) {
			PPU466::Tile tile;
			uint8_t t = uint8_t((255 * index) / 16);
			for (uint32_t y = 0; y < 8; ++y) {
				uint8_t bit0 = 0;
				uint8_t bit1 = 0;
				for (uint32_t x = 0; x < 8; ++x) {
					uint8_t d = distance[x+8*y];
					if (d > t) {
						bit0 |= (1 << x);
					} else {
						bit1 |= (1 << x);
					}
				}
				tile.bit0[y] = bit0;
				tile.bit1[y] = bit1;
			}
			ppu.tile_table[index] = tile;
		}
	}
	for(int i=0;i<enemy.size();++i){//Initialize Enemy
		int direction=rand()%4+1;
		int randomx=rand()%240;
		int randomy=rand()%240;
		enemy[i].enemy_at=glm::vec2(randomx,randomy);
		enemy[i].EnemyDirection=direction;
		enemy[i].isActive=false;
	}
	enemy[0].isActive=true;
	//use sprite 32 as a "player":



	ppu.palette_table[3]=GetPal("../assets/Tank_Pal.png");

	glm::uvec2* size=new glm::uvec2(8,8);
	std::vector< glm::u8vec4 > data1=GetSprite("../assets/Tank1.png", size);
	CalculateBit(data1,ppu.palette_table[3], &ppu.tile_table[35]);
	std::vector< glm::u8vec4 > data2=GetSprite("../assets/Tank2.png", size);
	CalculateBit(data2,ppu.palette_table[3], &ppu.tile_table[36]);
	std::vector< glm::u8vec4 > data3=GetSprite("../assets/Tank3.png", size);
	CalculateBit(data3,ppu.palette_table[3], &ppu.tile_table[37]);
	std::vector< glm::u8vec4 > data4=GetSprite("../assets/Tank4.png", size);
	CalculateBit(data4,ppu.palette_table[3], &ppu.tile_table[38]);
	std::vector< glm::u8vec4 > data5=GetSprite("../assets/Tank5.png", size);
	CalculateBit(data5,ppu.palette_table[3], &ppu.tile_table[39]);
	std::vector< glm::u8vec4 > data6=GetSprite("../assets/Tank6.png", size);
	CalculateBit(data6,ppu.palette_table[3], &ppu.tile_table[40]);
	std::vector< glm::u8vec4 > data7=GetSprite("../assets/Tank7.png", size);
	CalculateBit(data7,ppu.palette_table[3], &ppu.tile_table[41]);
	std::vector< glm::u8vec4 > data8=GetSprite("../assets/Tank8.png", size);
	CalculateBit(data8,ppu.palette_table[3], &ppu.tile_table[42]);

	ppu.palette_table[4]=GetPal("../assets/bullet_Pal.png");//load Bullet
	std::vector< glm::u8vec4 > data9=GetSprite("../assets/bullet.png", size);
	CalculateBit(data9,ppu.palette_table[4], &ppu.tile_table[43]);

	ppu.palette_table[5]=GetPal("../assets/Test_Pal.png");//load Enemy
	std::vector< glm::u8vec4 > data10=GetSprite("../assets/Test.png", size);
	CalculateBit(data10,ppu.palette_table[5], &ppu.tile_table[44]);


	/*ppu.tile_table[32].bit0 = {
		0b01111110,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b01111110,
	};
	ppu.tile_table[32].bit1 = {
		0b00000000,
		0b00000000,
		0b00011000,
		0b00100100,
		0b00000000,
		0b00100100,
		0b00000000,
		0b00000000,
	};

	//use sprite 33 as a "Bullet":
	ppu.tile_table[33].bit0 = {
		0b00000000,
		0b00011000,
		0b00111100,
		0b01111110,
		0b01111110,
		0b00111100,
		0b00011000,
		0b00000000,
	};
	ppu.tile_table[33].bit1 = {
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000,
	};

	//use sprite 34 as a "Enemy":
	ppu.tile_table[34].bit0 = {
		0b01111110,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b01111110,
	};
	ppu.tile_table[34].bit1 = {
		0b00000000,
		0b00000000,
		0b00011000,
		0b00100100,
		0b00000000,
		0b00100100,
		0b00000000,
		0b00000000,
	};

	//makes the outside of tiles 0-16 solid:
	ppu.palette_table[0] = {
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),
	};

	//makes the center of tiles 0-16 solid:
	ppu.palette_table[1] = {
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),
	};

	//used for the player:
	ppu.palette_table[7] = {
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0xff, 0xff, 0x00, 0xff),
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),
	};

	//used for the misc other sprites:
	ppu.palette_table[6] = {
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),//color1
		glm::u8vec4(0x88, 0x88, 0xff, 0xff),//color2
		glm::u8vec4(0x00, 0x00, 0x00, 0xff),//color3
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),//color4
	};*/
	ppu.palette_table[2] = {//no color
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
		glm::u8vec4(0x00, 0x00, 0x00, 0x00),
	};

}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.downs += 1;
			down.pressed = true;
			return true;
		}else if(evt.key.keysym.sym==SDLK_SPACE){
			space.downs+=1;

			space.pressed=true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.pressed = false;
			return true;
		}else if(evt.key.keysym.sym==SDLK_SPACE){
			space.pressed=false;
			return true;
		}
	}

	return false;
}

void PlayMode::emitBullet(glm::vec2 pos,int direction){
//1 find an not available Bullet;
	Bullet* toFind=nullptr;
	for(int i=0;i<bullet.size();++i){
		if(bullet[i].isActive==false){
			toFind=&bullet[i];
			break;
		}
	}
	if(toFind==nullptr){//available Bullet not exist
		return;
	}else{
		toFind->Init(pos,direction);
	//	std::cout<<"TEE"<<std::endl;
	}

}
void PlayMode::update(float elapsed) {

	//slowly rotates through [0,1):
	// (will be used to set background color)
	background_fade += elapsed / 10.0f;
	background_fade -= std::floor(background_fade);

	constexpr float PlayerSpeed = 30.0f;
	constexpr float EnemySpeed=30.0f;
	constexpr float BulletSpeed=150.0f;
	if (left.pressed) {
		player_at.x -= PlayerSpeed * elapsed;
		playerDirection=1;
	}
	if (right.pressed){
		player_at.x += PlayerSpeed * elapsed;
		playerDirection=2;
	} 
	if (down.pressed){
		player_at.y -= PlayerSpeed * elapsed;
		playerDirection=3;
	} 
	if (up.pressed){
		player_at.y += PlayerSpeed * elapsed;
		playerDirection=4;
	} 

	if(space.downs==1){
		//	std::cout<<"SpacePressed"<<std::endl;
			emitBullet(player_at,playerDirection);
	}

	//currentEnemyPositionUpdate
	for(int i=0;i<enemy.size();++i){
		if(enemy[i].isActive==true){
				if (enemy[i].EnemyDirection==1) enemy[i].enemy_at.x -= EnemySpeed * elapsed;
				if (enemy[i].EnemyDirection==2) enemy[i].enemy_at.x += EnemySpeed * elapsed;
				if (enemy[i].EnemyDirection==3) enemy[i].enemy_at.y -= EnemySpeed * elapsed;
				if (enemy[i].EnemyDirection==4) enemy[i].enemy_at.y += EnemySpeed * elapsed;

				if(enemy[i].enemy_at.x>240||enemy[i].enemy_at.x<0||enemy[i].enemy_at.y>240||enemy[i].enemy_at.y<0){//flip if meet boundary
					if (enemy[i].EnemyDirection==1) enemy[i].EnemyDirection=2;
					else if (enemy[i].EnemyDirection==2) enemy[i].EnemyDirection=1;
					else if (enemy[i].EnemyDirection==3) enemy[i].EnemyDirection=4;
					else if (enemy[i].EnemyDirection==4) enemy[i].EnemyDirection=3;
				}
		}
	}

	//currentBulletPositionUpdate
	for(int i=0;i<bullet.size();++i){
		if(bullet[i].isActive==true){
			//std::cout<<bullet[i].BulletDirection<<std::endl;
				if (bullet[i].BulletDirection==1) bullet[i].bullet_at.x -= BulletSpeed * elapsed;
				else if (bullet[i].BulletDirection==2) bullet[i].bullet_at.x += BulletSpeed * elapsed;
				else if (bullet[i].BulletDirection==3) bullet[i].bullet_at.y -= BulletSpeed * elapsed;
				else if (bullet[i].BulletDirection==4) bullet[i].bullet_at.y += BulletSpeed * elapsed;
				if(bullet[i].bullet_at.x>240||bullet[i].bullet_at.x<=0){
					bullet[i].isActive=false;
					bullet[i].bullet_at=glm::vec2(0,600);
				}else
				if(bullet[i].bullet_at.y>240||bullet[i].bullet_at.y<=0){
					bullet[i].isActive=false;
					bullet[i].bullet_at=glm::vec2(0,600);
				}		//deactivate bullet if it is out of bound;

				for(int j=0;j<enemy.size();++j){
					if(enemy[j].isActive){				// Collide Check
						glm::vec2 temp=enemy[j].enemy_at-bullet[i].bullet_at;
						float distance=glm::length(temp);
						std::cout<<temp.x<<"   "<<temp.y<<"Distance"<<distance<<std::endl;
						if(distance<10){
							enemy[j].isActive=false;
							bullet[i].isActive=false;
							enemy[j].enemy_at=glm::vec2(0,600);
						}
					}
					

				}
		}

	}
	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
	space.downs=0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//--- set ppu state based on game state ---

	//background color will be some hsv-like fade:
	ppu.background_color = glm::u8vec4(
		std::min(255,std::max(0,int32_t(255 * 0.5f * (0.5f + std::sin( 2.0f * M_PI * (background_fade + 0.0f / 3.0f) ) ) ))),
		std::min(255,std::max(0,int32_t(255 * 0.5f * (0.5f + std::sin( 2.0f * M_PI * (background_fade + 1.0f / 3.0f) ) ) ))),
		std::min(255,std::max(0,int32_t(255 * 0.5f * (0.5f + std::sin( 2.0f * M_PI * (background_fade + 2.0f / 3.0f) ) ) ))),
		0xff
	);

	//tilemap gets recomputed every frame as some weird plasma thing:
	//NOTE: don't do this in your game! actually make a map or something :-)
	/*for (uint32_t y = 0; y < PPU466::BackgroundHeight; ++y) {
		for (uint32_t x = 0; x < PPU466::BackgroundWidth; ++x) {
			//TODO: make weird plasma thing
			ppu.background[x+PPU466::BackgroundWidth*y] = ((x+y)%16);
		}
	}*/

	//background scroll:
	//ppu.background_position.x = int32_t(-0.5f * player_at.x);
	//ppu.background_position.y = int32_t(-0.5f * player_at.y);
	//		ppu.sprites[0].x = int8_t(player_at.x);
	//		ppu.sprites[0].y = int8_t(player_at.y);
	//		ppu.sprites[0].index = 35;
	//		ppu.sprites[0].attributes = 3;

	for(uint8_t i=0;i<2;++i){	//player sprite:
		for(uint8_t j=0;j<4;++j){
			ppu.sprites[i*4+j].x = int8_t(player_at.x+8*j);
			ppu.sprites[i*4+j].y = int8_t(player_at.y-8*i);
			ppu.sprites[i*4+j].index = 35+i*4+j;
			ppu.sprites[i*4+j].attributes = 3;
		}
	}



	//bullet sprite render:
	for(int i=0;i<8;++i){
		//if(bullet[i].isActive==true){
			ppu.sprites[10+i].x=int8_t(bullet[i].bullet_at.x);
			ppu.sprites[10+i].y=int8_t(bullet[i].bullet_at.y);
			ppu.sprites[10+i].index=43;
			if(bullet[i].isActive==true){
				ppu.sprites[10+i].attributes=4;
			}else{
				ppu.sprites[10+i].attributes=2;
			}

		//}
	}

	/*
	for(int i=0;i<8;++i){
		//if(enemy[i].isActive==true){
			ppu.sprites[20+i].x=int8_t(enemy[i].enemy_at.x);
			ppu.sprites[20+i].y=int8_t(enemy[i].enemy_at.y);
			ppu.sprites[20+i].index=42;
			if(enemy[i].isActive==true){
				ppu.sprites[20+i].attributes=5;
			}else{
				ppu.sprites[20+i].attributes=2;
			}

		//}
	}
*/

	//some other misc sprites:
	/*for (uint32_t i = 1; i < 63; ++i) {
		float amt = (i + 2.0f * background_fade) / 62.0f;
		ppu.sprites[i].x = int8_t(0.5f * PPU466::ScreenWidth + std::cos( 2.0f * M_PI * amt * 5.0f + 0.01f * player_at.x) * 0.4f * PPU466::ScreenWidth);
		ppu.sprites[i].y = int8_t(0.5f * PPU466::ScreenHeight + std::sin( 2.0f * M_PI * amt * 3.0f + 0.01f * player_at.y) * 0.4f * PPU466::ScreenWidth);
		ppu.sprites[i].index = 32;
		ppu.sprites[i].attributes = 6;
		if (i % 2) ppu.sprites[i].attributes |= 0x80; //'behind' bit
	}*/

	//--- actually draw ---
	ppu.draw(drawable_size);
}

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
}
