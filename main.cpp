#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

int cellsize=30;
int cellcount=30;

bool elementindeque(Vector2 element, deque<Vector2> deque){
    for(int i=0;i<deque.size();i++){
        if(Vector2Equals(element,deque[i]))
            return false;
    }
    return true;

}

bool collision(Vector2 element, deque<Vector2> deque){
    for(int i=1;i<deque.size();i++){
        if(Vector2Equals(element,deque[i]))
            return true;
    }
    return false;
}

class Snake{
    public:
    deque<Vector2> body ={Vector2{6,9},Vector2{5,9},Vector2{4,9}};
    Vector2 direction = {1,0};
    bool addsegment = false;

    void Draw(){
        for(int i=0;i< body.size();i++){
            int x=body[i].x;
            int y=body[i].y;
            DrawCircle(x*cellsize,y*cellsize,15,WHITE);
        }
    }

    void update(){
      
        body.push_front(Vector2Add(body[0],direction));
        
        if(addsegment == true){    
            addsegment = false;
        }else{
            body.pop_back();
        }    
    }

    void reset(){
        body ={Vector2{6,9},Vector2{5,9},Vector2{4,9}};
        direction = {1,0};
    }
};

class Food{
    public:
    Snake snake = Snake();
    Vector2 position = updatefood();
    deque<Vector2> snakebody = snake.body;

    void Draw(){
        DrawCircle(position.x*cellsize,position.y*cellsize,15,RED);
    }

    Vector2 updatefood(){
        float x=rand()%29;
        float y=rand()%29;

        if(x>0 && y>0 && elementindeque({x,y},snakebody))
            return Vector2{x,y};
        else
            updatefood();

        return {0,0};

    }

};

int main () {

    InitWindow(cellcount*cellsize, cellcount*cellsize, "Retro Snake");
    SetTargetFPS(8);

    Food food = Food();
    Snake snake = Snake();

    int score=0;
    bool running = true;
    
    while(!WindowShouldClose()){


        BeginDrawing();
        
        if(running)
            snake.update();


        if(IsKeyPressed(KEY_UP) && snake.direction.y != 1){
            snake.direction = {0,-1};
            running = true;
        }
        if(IsKeyPressed(KEY_DOWN) && snake.direction.y != -1){
            snake.direction = {0,1};
            running = true;
        }
        if(IsKeyPressed(KEY_RIGHT) && snake.direction.x != -1){
            snake.direction = {1,0};
            running = true;
        }
        if(IsKeyPressed(KEY_LEFT) && snake.direction.x != 1){
            snake.direction = {-1,0};
            running = true;
        }

        // collision between snake and food //
        if(Vector2Equals(snake.body[0],food.position)){
            food.position = food.updatefood();
            snake.addsegment = true;
            score++;
        }
        
        //collision between snake and edge //
        if(snake.body[0].x == cellcount || snake.body[0].x == -1){
            snake.reset();
            food.position = food.updatefood();
            running = false;
            score = 0;
        }
        if(snake.body[0].y == cellcount || snake.body[0].y == -1){
            snake.reset();
            food.position = food.updatefood();
            running = false;
            score = 0;
        }

        if(collision(snake.body[0],snake.body)){
            snake.reset();
            food.position = food.updatefood();
            running = false;
            score = 0;
        }
    

        ClearBackground(BLACK);

        food.Draw();
        snake.Draw();
        DrawText(TextFormat("%i",score),450,5,50,WHITE);
        

        EndDrawing();
    }

    CloseWindow();
    return 0;
}