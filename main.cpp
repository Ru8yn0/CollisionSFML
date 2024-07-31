#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Window {
public:
    Window() {
        setup("Window", sf::Vector2u(800, 600));
    }
    Window(const std::string l_title, const sf::Vector2u l_size) {
        setup(l_title, l_size);
    }
    ~Window() {
        Destroy();
    }

    void BeginDraw() {
        p_window.clear(sf::Color::Black);
    }
    void EndDraw() {
        p_window.display();
    }
    void Update() {
        sf::Event event;
        while (p_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                p_isDone = true;
            }
        }
    }
    sf::Vector2u WindowGetSize() {
        return p_size;
    }
    void ToggleFullScreen() {
        p_isFullScreen = !p_isFullScreen;
        Destroy();
        Create();
    }

    void HandlePlayer1Movement(sf::RectangleShape& player, float deltaspeed, float movementspeed, sf::Vector2f& speed) {
    
    sf::FloatRect PlayerBounds = player.getGlobalBounds();
    
    
    speed.x = 0;
    speed.y = 0;

   
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        speed.y -= movementspeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        speed.x -= movementspeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        speed.y += movementspeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        speed.x += movementspeed;
    }

   
    player.move(speed * deltaspeed);

      
    for (const auto& wall : walls) {
        sf::FloatRect WallBounds = wall.getGlobalBounds();

        if (PlayerBounds.intersects(WallBounds)) {
            
            float overlapLeft = PlayerBounds.left + PlayerBounds.width - WallBounds.left;
            float overlapRight = WallBounds.left + WallBounds.width - PlayerBounds.left;
            float overlapTop = PlayerBounds.top + PlayerBounds.height - WallBounds.top;
            float overlapBottom = WallBounds.top + WallBounds.height - PlayerBounds.top;

            float overlapX = std::min(overlapLeft, overlapRight);
            float overlapY = std::min(overlapTop, overlapBottom);

            
            
            if (overlapX < overlapY) {
                if (PlayerBounds.left < WallBounds.left) {
                    player.move(-overlapX, 0);  
                } else {
                    player.move(overlapX, 0);   
                }
            } else {
                if (PlayerBounds.top < WallBounds.top) {
                    player.move(0, -overlapY);  
                } else {
                    player.move(0, overlapY);   
                }
            }
        }
    }
}


    void DrawWalls(){
        for(const auto& wall: walls){
            p_window.draw(wall);
        }
    }

    bool isFullScreen() { return p_isFullScreen; }
    bool isDone() { return p_isDone; }
    void Draw(sf::Drawable& l_drawable) {
        p_window.draw(l_drawable);
    }
private:
    void setup(const std::string l_title, const sf::Vector2u l_size) {
        p_title = l_title;
        p_size = l_size;
        p_isFullScreen = false;
        p_isDone = false;
        InitializeWalls();
        Create();
        
    }
        void InitializeWalls(){

       std::vector<sf::Vector2f> Position = {
            {100.f, 100.f},
            {200.f, 150.f},
            {300.f, 200.f},
            {400.f, 100.f},
            {500.f, 150.f},
            {600.f, 200.f},
            {700.f, 100.f},
        };

        std::vector<sf::Vector2f> Size = {
            {50.f, 50.f},
            {100.f, 50.f},
            {50.f, 100.f},
            {50.f, 50.f},
            {100.f, 50.f},
            {50.f, 100.f},
            {50.f, 50.f},
        };

       for(int i = 0; i < Position.size(); ++i){
        sf::RectangleShape wall(Size[i]);
        wall.setPosition(Position[i]);
        wall.setFillColor(sf::Color::Red);
        walls.push_back(wall);
       }
       
    }
    void Create() {
        auto style = (p_isFullScreen ? sf::Style::Fullscreen : sf::Style::Default);
        p_window.create(sf::VideoMode(p_size.x, p_size.y), p_title, style);
    }
    void Destroy() {
        p_window.close();
    }

    sf::RenderWindow p_window;
    std::string p_title;
    sf::Vector2u p_size;
    bool p_isFullScreen;
    bool p_isDone;
    std::vector<sf::RectangleShape> walls;
};



int main(int argc, char** argv) {
    Window window("Window", sf::Vector2u(800, 600));
    sf::RectangleShape player(sf::Vector2f(70.0f, 60.0f)); // Initial size
    player.setFillColor(sf::Color::Green);
    player.setPosition(375.0f, 275.0f);
    sf::RectangleShape wall(sf::Vector2f(100,30));
    wall.setFillColor(sf::Color::Red);
    sf::Clock clock;
    float movementspeed = 100.0f;
    sf::Vector2f speed;

    while (!window.isDone()) {
        sf::Time deltaTime = clock.restart();
        float deltaspeed = deltaTime.asSeconds();

        window.Update();
        window.BeginDraw();
        window.HandlePlayer1Movement(player, deltaspeed, movementspeed, speed);    
        window.Draw(player);
        window.DrawWalls(); 
        window.EndDraw();
    }


    return 0;
}
