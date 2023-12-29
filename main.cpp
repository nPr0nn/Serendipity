
#include "headers/utils.h"
#include "headers/colors.h"
#include "headers/sprite.h"

#include "game.cpp"
#include <raylib.h>

typedef void (*main_loop_ptr)(void*);
#ifdef PLATFORM_WEB 
  #include <emscripten.h>
  void run(void* ctx, main_loop_ptr func){
    emscripten_set_main_loop_arg(func, ctx, -1, 1);   
  }
#else 
  void run(void* ctx, main_loop_ptr func){
    std::cout << "PUTZ" << std::endl;
  }
#endif


void UpdateDrawFrame(void* args);     // Update and Draw one frame

struct game_ctx{
  int w, h;
  const char* title;
  bool vic, def; 
  Music m;
  Vector2 screen_target;
  Vector2 target;
  Rectangle box;
  Sprite cursor;
  Camera2D cam;
};

int32_t main(void) {
  
  const char* title = "Serendipity";
  
  // Submerged Chimera Pallete (https://lospec.com/palette-list/submerged-chimera)
  l1 = color_from_hex("#f7faea");
  l2 = color_from_hex("#ffd2de");  
  l3 = color_from_hex("#f792e4");
  l4 = color_from_hex("#de39e9"); 

  d1 = color_from_hex("#8623ae");
  d2 = color_from_hex("#6d4299"); 
  d3 = color_from_hex("#2e1e5c");
  d4 = color_from_hex("#120f28"); 

  game_ctx ctx;
  ctx.title = title;
  
   // Window
  int window_width  = 2*OFFSET + WIDTH*CELLSIZE;
  int window_height = 2*OFFSET + HEIGHT*CELLSIZE;
  std::cout << window_width << " " << window_height << "\n";
  ctx.w = window_width;
  ctx.h = window_height;
  InitWindow(window_width, window_height, title);
  SetTargetFPS(60); 

  // Cursor 
  ctx.cursor = {LoadTexture("assets/target.png"), {0, 0, 28, 28}, {0, 0}};
  ctx.target = {0, 0};
  ctx.screen_target = {0, 0};
  HideCursor();

  // Game Box
  ctx.box = { (OFFSET), (OFFSET + 20), (CELLSIZE*WIDTH), (CELLSIZE*HEIGHT) };

  // Camera
  Camera2D camera = { 0 };
  camera.target = {0, 0};
  camera.offset = {(float)window_width/2, (float)window_height/2};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;
  ctx.cam = camera;

  InitAudioDevice();
  // Music
  ctx.m = LoadMusicStream("assets/music.wav");
  PlayMusicStream(ctx.m);

  // Game
  int number_of_boids = 0;
  ctx.vic = false;
  ctx.def  = false;
  game_init(window_width, window_height, camera);

  run(&ctx, UpdateDrawFrame);
 
  while(!WindowShouldClose()){
    UpdateDrawFrame(&ctx);
  }

  UnloadMusicStream(ctx.m);
  CloseAudioDevice();
  CloseWindow();
}

void UpdateDrawFrame(void* args){

  game_ctx* ctx = (game_ctx*) args;
  int window_height = ctx->h;
  int window_width =  ctx->w;
  bool victory = ctx->vic;
  bool defeat  = ctx->def;
  
  UpdateMusicStream(ctx->m);
  
  ctx->screen_target = GetMousePosition();
  ctx->target = GetScreenToWorld2D(ctx->screen_target, ctx->cam);

  // Move Screen
  if(IsMouseButtonDown(0)){  
    Vector2 delta_target = ctx->screen_target - ctx->cam.offset;
    setMag(delta_target, 5);
    ctx->cam.target = ctx->cam.target + delta_target;
  }

  // Update
  if(!(victory || defeat)){
    number_of_boids = update_loop(ctx->box, ctx->target, window_width, window_height, ctx->cam);
  }
  if(number_of_boids >= 200) victory = true;
  if(number_of_boids <= 0)   defeat  = true;

  BeginDrawing();

  if(!(victory || defeat)){
    ClearBackground(d3);
    BeginMode2D(ctx->cam);
      draw_loop(ctx->target);
    EndMode2D();
  }
  else if(victory){
    ClearBackground(l1);
    d3 = l1;
    const char* vic = "Parabéns, você salvou todos \n os passarinhos";
    DrawText(vic, window_width/2 - MeasureText(vic, 25)/2 , window_height/2, 25, l3);
  }
  else if(defeat){
    ClearBackground(d4);
    d3 = d4;
    const char* vic = "Oh no, você condenou a todos nós...";        
    DrawText(vic, window_width/2 - MeasureText(vic, 25)/2 , window_height/2, 25, l3);  
  }

  Sprite cursor = ctx->cursor;
  DrawTextureRec(cursor.texture, cursor.rect, ctx->screen_target - (Vector2){cursor.rect.width/2, cursor.rect.height/2}, WHITE);
  DrawRectangle(0, 0, window_width, OFFSET + 20, d3);
  DrawRectangle(0, window_height-OFFSET+20, window_width, OFFSET, d3);
  DrawRectangle(0, 0, OFFSET, window_height, d3);
  DrawRectangle(window_width - OFFSET, 0, OFFSET, window_height, d3);
  DrawRectangleRoundedLines(ctx->box, 0, 0, 5, l3);

  std::string sep = " : ";
  std::string full_title = (ctx->title + sep + std::to_string(number_of_boids)) + " / 200";
  DrawText(full_title.c_str(), window_width/2 - MeasureText("paosaosasaosiasaasass", 40)/2 , 20, 40, l3);
  EndDrawing();

}

