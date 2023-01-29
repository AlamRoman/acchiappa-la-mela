#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <stdbool.h>
#include <time.h>

#define max_mela 100

struct oggetto {
	int size;
	int pos_x;
	int pos_y;
	int vx;
	int vy;
	Texture texture;
};

int main(){

	InitWindow(800,450,"Acchiappa la mela");
	SetTargetFPS(60);

	InitAudioDevice();
	Music music = LoadMusicStream("audio/music.mp3");
	PlayMusicStream(music);

	Sound gameover_sound = LoadSound("audio/glitch.mp3");

	Sound collect =  LoadSound("audio/collect.mp3");

	Image image;

	image = LoadImage("img/home.png");
	ImageResize(&image,800,450);
	Texture Home_bg=LoadTextureFromImage(image);

	image = LoadImage("img/bg.png");
	ImageResize(&image,800,450);
	Texture game_bg=LoadTextureFromImage(image);

	image = LoadImage("img/gameover.png");
	ImageResize(&image,300,300);
	Texture gamover_image=LoadTextureFromImage(image);

	int n_mela=10;
	int n_acqua=5;
	int i;
	int punteggio=0;
	bool gameover = 1;

	struct oggetto android;
	struct oggetto mela[max_mela];
	struct oggetto acqua[n_acqua];

	//android

	android.size=100;
	android.pos_x=GetRandomValue(0,GetScreenWidth()-android.size);
	android.pos_y=GetRandomValue(0,GetScreenHeight()-android.size);
	android.vx=0;
	android.vy=0;
	image=LoadImage("img/android.png");
	ImageResize(&image,android.size,android.size);
	android.texture=LoadTextureFromImage(image);

	//mela

	mela[0].size=30;
	image=LoadImage("img/apple.png");
	ImageResize(&image,mela[0].size,mela[0].size);

	for(i=0;i<n_mela;i++){
		mela[i].size=30;
		mela[i].pos_x=GetRandomValue(0,GetScreenWidth()-mela[i].size);
		mela[i].pos_y=GetRandomValue(0,GetScreenHeight()-mela[i].size);
		mela[i].vx=GetRandomValue(1,4);
		mela[i].vy=GetRandomValue(1,4);
		mela[i].texture=LoadTextureFromImage(image);
	}

	//acqua

	acqua[0].size=30;
	image=LoadImage("img/acqua.png");
	ImageResize(&image,mela[0].size,mela[0].size);

	for(i=0;i<n_acqua;i++){
		acqua[i].size=30;
		acqua[i].pos_x=GetRandomValue(acqua[i].size,GetScreenWidth()-acqua[i].size);
		acqua[i].pos_y=GetRandomValue(-100,-2000);
		acqua[i].vx=0;
		acqua[i].vy=1;
		acqua[i].texture=LoadTextureFromImage(image);
	}

	while(!WindowShouldClose()){

        UpdateMusicStream(music);

		if(!gameover){
			android.pos_x+=android.vx;
			android.pos_y+=android.vy;

            //mela
			for(i=0;i<n_mela;i++){
				mela[i].pos_x+=mela[i].vx;
				mela[i].pos_y+=mela[i].vy;

				if(mela[i].pos_x<=0){
					mela[i].vx*=-1;
					mela[i].pos_x=0;
				}else if(mela[i].pos_x>=GetScreenWidth()-mela[i].size){
					mela[i].vx*=-1;
					mela[i].pos_x=GetScreenWidth()-mela[i].size;
				}

				if(mela[i].pos_y<=0){
					mela[i].vy*=-1;
					mela[i].pos_y=0;
				}else if(mela[i].pos_y>=GetScreenHeight()-mela[i].size){
					mela[i].vy*=-1;
					mela[i].pos_y=GetScreenHeight()-mela[i].size;
				}

                if((mela[i].pos_x>android.pos_x && mela[i].pos_x<android.pos_x+android.size - android.size/4) && (mela[i].pos_y>android.pos_y && mela[i].pos_y<android.pos_y+android.size - android.size/4)){
					PlaySound(collect);
					punteggio++;
					mela[i].pos_x=GetRandomValue(0,GetScreenWidth()-mela[i].size);
					mela[i].pos_y=GetRandomValue(0,GetScreenHeight()-mela[i].size);
					mela[i].vx=GetRandomValue(1,4);
					mela[i].vy=GetRandomValue(1,4);
				}
			}

            //acqua
			for(i=0;i<n_acqua;i++){
				acqua[i].pos_y+=acqua[i].vy;

				if(acqua[i].pos_y>GetScreenHeight()+50){
					acqua[i].pos_x=GetRandomValue(acqua[i].size,GetScreenWidth()-acqua[i].size);
					acqua[i].pos_y=GetRandomValue(-100,-2000);
				}

				if((acqua[i].pos_x>android.pos_x && acqua[i].pos_x<android.pos_x+android.size - android.size/4) && (acqua[i].pos_y>android.pos_y && acqua[i].pos_y<android.pos_y+android.size - android.size/4)){
                    //gameover
                    PauseMusicStream(music);
					PlaySound(gameover_sound);

                    BeginDrawing();
                        DrawTexture(gamover_image,GetScreenWidth()/2-150,GetScreenHeight()/2-150,WHITE);
					EndDrawing();

					sleep(3);
					ResumeMusicStream(music);

					gameover=true;

                    image=LoadImage("img/acqua.png");
                    ImageResize(&image,acqua[0].size,acqua[0].size);

                    //reset acqua
					for(i=0;i<n_acqua;i++){
						acqua[i].size=30;
						acqua[i].pos_x=GetRandomValue(acqua[i].size,GetScreenWidth()-acqua[i].size);
						acqua[i].pos_y=GetRandomValue(-100,-2000);
						acqua[i].vx=0;
						acqua[i].vy=1;
						acqua[i].texture=LoadTextureFromImage(image);
					}
				}
			}

			//controllo bordi
			if(android.pos_x<=-25){
				android.vx=0;
				android.pos_x=-25;
			}else if(android.pos_x>=GetScreenWidth()-android.size+25){
				android.vx=0;
				android.pos_x=GetScreenWidth()-android.size+25;
			}

			if(android.pos_y<=-5){
				android.vy=0;
				android.pos_y=-5;
			}else if(android.pos_y>=GetScreenHeight()-android.size+25){
				android.vy=0;
				android.pos_y=GetScreenHeight()-android.size+25;
			}

            //movimenti
			if(IsKeyDown(KEY_UP)){
				android.vy=-5;
			}else if(IsKeyDown(KEY_DOWN)){
				android.vy=5;
			}else{
				android.vy=0;
			}

			if(IsKeyDown(KEY_LEFT)){
				android.vx=-5;
			}else if(IsKeyDown(KEY_RIGHT)){
				android.vx=5;
			}else{
				android.vx=0;
			}

            //aggiunge e rimuove mela
			if(IsKeyPressed(KEY_A) && n_mela<=max_mela){

				image=LoadImage("img/apple.png");
				ImageResize(&image,mela[0].size,mela[0].size);

				mela[n_mela].size=30;
				mela[n_mela].pos_x=GetRandomValue(0,GetScreenWidth()-mela[n_mela].size);
				mela[n_mela].pos_y=GetRandomValue(0,GetScreenHeight()-mela[n_mela].size);
				mela[n_mela].vx=GetRandomValue(1,6);
				mela[n_mela].vy=GetRandomValue(1,6);
				mela[n_mela].texture=LoadTextureFromImage(image);
				n_mela++;

			}else if(IsKeyPressed(KEY_R)){
				if(n_mela>0){
					n_mela--;
				}
			}

			BeginDrawing();

				ClearBackground(WHITE);

				DrawTexture(game_bg,0,0,WHITE);

				for(i=0;i<n_mela;i++){
					DrawTexture(mela[i].texture,mela[i].pos_x,mela[i].pos_y,WHITE);
				}

				for(i=0;i<n_acqua;i++){
					DrawTexture(acqua[i].texture,acqua[i].pos_x,acqua[i].pos_y,WHITE);
				}

				DrawTexture(android.texture,android.pos_x,android.pos_y,WHITE);
				DrawText(TextFormat("Score: %i", punteggio), GetScreenWidth()-125, 10, 20, BLACK);

			EndDrawing();

		}else{

            //tempo
			time_t s, val = 1;
   			struct tm* curr_time;
   			s = time(NULL);
   			curr_time = localtime(&s);

   			//pulsante gioca
			if(
				(CheckCollisionPointTriangle(
						GetMousePosition(),
						(Vector2){ GetScreenWidth()/2-50, GetScreenHeight()/2},
						(Vector2){ GetScreenWidth()/2-50, GetScreenHeight()/2 + 150},
						(Vector2){ GetScreenWidth()/2+75, GetScreenHeight()/2 + 75})
				) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			){
				gameover=false;
				punteggio=0;
				n_mela=10;
			}

			BeginDrawing();

				ClearBackground(WHITE);

				DrawTexture(Home_bg,0,0,WHITE);

				DrawCircle(75,75,75,PURPLE);

				DrawRectangle(GetScreenWidth()/2-100,GetScreenHeight()/2-100,200,80,RED);
				DrawText(TextFormat("Score: %i", punteggio), GetScreenWidth()/2-85, GetScreenHeight()/2-75, 35, BLACK);

				DrawTriangle(
					(Vector2){ GetScreenWidth()/2-50, GetScreenHeight()/2},
					(Vector2){ GetScreenWidth()/2-50, GetScreenHeight()/2 + 150},
					(Vector2){ GetScreenWidth()/2+75, GetScreenHeight()/2 + 75},
					BLUE
				);

				DrawText(TextFormat("%02d:%02d:%02d", curr_time->tm_hour, curr_time->tm_min,curr_time->tm_sec),15,60,30,BLACK);

				DrawText("Gioca",GetScreenWidth()/2-40,GetScreenHeight()/2+60,30,WHITE);

				DrawText("Acchiappa la mela",GetScreenWidth()/2-130,25,30,RED);

			EndDrawing();

		}

	}

	UnloadTexture(Home_bg);
	UnloadTexture(game_bg);
	UnloadTexture(android.texture);

	for(i=0;i<n_mela;i++){
		UnloadTexture(mela[i].texture);
	}

	for(i=0;i<n_acqua;i++){
		UnloadTexture(acqua[i].texture);
	}

	UnloadMusicStream(music);
	UnloadSound(gameover_sound);
	UnloadSound(collect);

	CloseAudioDevice();

	CloseWindow();

	return 0;
}
