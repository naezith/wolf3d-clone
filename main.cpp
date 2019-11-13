#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#define mapWidth 24
#define mapHeight 24

static int worldMap[mapWidth][mapHeight] =
        {
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
        };

static double posX = 22, posY = 12;  //x and y start position
static double dirX = -1, dirY = 0; //initial direction vector
static double planeX = 0, planeY = 1.03; //the 2d raycaster version of camera plane

static const int w = 1280;
static const int h = 720;


int main()
{
    sf::RenderWindow window(sf::VideoMode(w, h), "SFML window");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    sf::Texture texture;
    sf::Image image;
    image.loadFromFile("another_wall.png");
    texture.loadFromFile("banner.png");
    static std::size_t tex_width = texture.getSize().x;
    static std::size_t tex_height = texture.getSize().y;

    sf::VertexArray lines(sf::Lines, w * 2);
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        std::cout <<  1 / elapsed.asSeconds() << std::endl;
        // Process events
        sf::Event event{};
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //speed modifiers
        double moveSpeed = elapsed.asSeconds() * 5.0; //the constant value is in squares/second
        double rotSpeed = elapsed.asSeconds() * 3.0; //the constant value is in radians/second

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            if (worldMap[int(posX + dirX * moveSpeed)][int(posY)] == 0) posX += dirX * moveSpeed;
            if (worldMap[int(posX)][int(posY + dirY * moveSpeed)] == 0) posY += dirY * moveSpeed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            if (worldMap[int(posX - dirX * moveSpeed)][int(posY)] == 0) posX -= dirX * moveSpeed;
            if (worldMap[int(posX)][int(posY - dirY * moveSpeed)] == 0) posY -= dirY * moveSpeed;
        }
        //rotate to the right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            //both camera direction and camera plane must be rotated
            double oldDirX = dirX;
            dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
            dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
            planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
        }
        //rotate to the left
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            //both camera direction and camera plane must be rotated
            double oldDirX = dirX;
            dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
            dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
            planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
        }

        for (int x = 0, idx_vx = 0; x < w; x++, idx_vx += 2) {
            //calculate ray position and direction
            double cameraX = 2 * x / double(w) - 1; //x-coordinate in camera space

            double rayDirX = dirX + planeX * cameraX;
            double rayDirY = dirY + planeY * cameraX;
            //which box of the map we're in
            int mapX = int(posX);
            int mapY = int(posY);

            //length of ray from current position to next x or y-side
            double sideDistX;
            double sideDistY;

            //length of ray from one x or y-side to next x or y-side
            //double deltaDistX = std::abs(1 / rayDirX);
            //double deltaDistY = std::abs(1 / rayDirY);
            double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
            double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

            double perpWallDist;

            //what direction to step in x or y-direction (either +1 or -1)
            int stepX;
            int stepY;

            int hit = 0; //was there a wall hit?
            int side; //was a NS or a EW wall hit?
            //calculate step and initial sideDist
            if (rayDirX < 0) {
                stepX = -1;
                sideDistX = (posX - mapX) * deltaDistX;
            } else {
                stepX = 1;
                sideDistX = (mapX + 1.0 - posX) * deltaDistX;
            }
            if (rayDirY < 0) {
                stepY = -1;
                sideDistY = (posY - mapY) * deltaDistY;
            } else {
                stepY = 1;
                sideDistY = (mapY + 1.0 - posY) * deltaDistY;
            }
            //perform DDA
            while (hit == 0) {
                //jump to next map square, OR in x-direction, OR in y-direction
                if (sideDistX < sideDistY) {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                } else {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }
                //Check if ray has hit a wall
                if (worldMap[mapX][mapY] > 0) hit = 1;
            }
            //Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
            if (side == 0) {
                perpWallDist = std::fabs((mapX - posX + (1 - stepX) / 2) / rayDirX);
            } else {
                perpWallDist = std::fabs((mapY - posY + (1 - stepY) / 2) / rayDirY);
            }

            //Calculate height of line to draw on screen
            int lineHeight = std::abs((int) (h / perpWallDist));

            //calculate lowest and highest pixel to fill in current stripe
            int drawStart = -lineHeight / 2 + h / 2;
            int drawEnd = lineHeight / 2 + h / 2;

            //calculate value of wallX
            double wallX; //where exactly the wall was hit
            if (side == 0) wallX = posY + perpWallDist * rayDirY;
            else wallX = posX + perpWallDist * rayDirX;
            wallX -= floor((wallX));

            //x coordinate on the texture
            int texX = int(wallX * double(tex_width));
            if (side == 0 && rayDirX > 0) texX = tex_width - texX - 1;
            if (side == 1 && rayDirY < 0) texX = tex_height - texX - 1;


            lines[idx_vx].texCoords = {float(texX), 0};
            lines[idx_vx + 1].texCoords = {float(texX), float(tex_height)};
            lines[idx_vx].position = {float(x), (float) drawStart};
            lines[idx_vx + 1].position = {float(x), (float) drawEnd};
        }

        // Clear screen
        window.clear(sf::Color::Black);
        window.draw(lines, &texture);
        window.display();
    }
    return EXIT_SUCCESS;
}
