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
    window.setVerticalSyncEnabled(true);
    sf::Texture texture;
    texture.setSmooth(true);
    texture.loadFromFile("csgo.png");
    sf::Vector2i floor_texture_index{0, 0};
    sf::Vector2i ceiling_texture_index{2, 2};
    sf::Vector2i wall_texture_index{2, 0};
    static std::size_t tex_width = 256;
    static std::size_t tex_height = 256;

    // 2 vertices for a line, 3 lines (ceiling wall floor)
    int vertice_count_per_column = 2;
    sf::VertexArray lines(sf::Lines, w * vertice_count_per_column);
    sf::VertexArray points(sf::Points);
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        std::cout << 1 / elapsed.asSeconds() << std::endl;
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            if (worldMap[int(posX + dirX * moveSpeed)][int(posY)] == 0) posX += dirX * moveSpeed;
            if (worldMap[int(posX)][int(posY + dirY * moveSpeed)] == 0) posY += dirY * moveSpeed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
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

        points.clear();
        for (int x = 0, idx_vx = 0; x < w; x++, idx_vx += vertice_count_per_column) {
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


            // Prepare wall line
            {
                sf::Vector2f offset{(float)wall_texture_index.x * (tex_width + 2) + 1, (float)wall_texture_index.y * (tex_height + 2) + 1};
                lines[idx_vx + 0].texCoords = offset + sf::Vector2f{float(texX), 0};
                lines[idx_vx + 1].texCoords = offset + sf::Vector2f{float(texX), (float)tex_height};
                lines[idx_vx + 0].position = {float(x), (float) drawStart};
                lines[idx_vx + 1].position = {float(x), (float) drawEnd};
            }


            //FLOOR CASTING
            double floorXWall, floorYWall; //x, y position of the floor texel at the bottom of the wall

            //4 different wall directions possible
            if(side == 0 && rayDirX > 0)
            {
                floorXWall = mapX;
                floorYWall = mapY + wallX;
            }
            else if(side == 0 && rayDirX < 0)
            {
                floorXWall = mapX + 1.0;
                floorYWall = mapY + wallX;
            }
            else if(side == 1 && rayDirY > 0)
            {
                floorXWall = mapX + wallX;
                floorYWall = mapY;
            }
            else
            {
                floorXWall = mapX + wallX;
                floorYWall = mapY + 1.0;
            }

            double distWall, distPlayer;

            distWall = perpWallDist;
            distPlayer = 0.0;

            //draw the floor from drawEnd to the bottom of the screen
            for(int y = drawEnd + 1; y < h; y++)
            {
                double currentDist = h / (2.0 * y - h); //you could make a small lookup table for this instead
                double weight = (currentDist - distPlayer) / (distWall - distPlayer);
                double currentFloorX = weight * floorXWall + (1.0 - weight) * posX;
                double currentFloorY = weight * floorYWall + (1.0 - weight) * posY;

                int floorTexX, floorTexY;
                floorTexX = int(currentFloorX * tex_width) % tex_width;
                floorTexY = int(currentFloorY * tex_height) % tex_height;

                // Prepare floor
                {
                    sf::Vector2f offset{(float)floor_texture_index.x * (tex_width + 2) + 1, (float)floor_texture_index.y * (tex_height + 2) + 1};
                    sf::Vertex vertex({ (float)x - 1, (float)y }, offset + sf::Vector2f{(float)floorTexX, (float)floorTexY});
                    points.append(vertex);
                }

                // Prepare ceiling
                {
                    sf::Vector2f offset{(float)ceiling_texture_index.x * (tex_width + 2) + 1, (float)ceiling_texture_index.y * (tex_height + 2) + 1};
                    sf::Vertex vertex({ (float)x - 1, (float)h - y + 1 }, offset + sf::Vector2f{(float)floorTexX, (float)floorTexY});
                    points.append(vertex);
                }
            }
        }

        // Clear screen
        window.clear(sf::Color::Black);
        window.draw(points, &texture);
        window.draw(lines, &texture);
        window.display();
    }
    return EXIT_SUCCESS;
}
