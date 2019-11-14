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
static double fov_degrees = 103;
static double fov = fov_degrees / 100.0;
static double planeX = 0, planeY = fov; //the 2d raycaster version of camera plane

static const int w = 1280;
static const int h = 720;
static const float mouse_sensitivity = 0.3f;

static float magnitude(const sf::Vector2f& a) {
    return sqrt(a.x * a.x + a.y * a.y);
}

constexpr float RAD2DEG = 57.295779513082320876798154814105f;
static float vecToAngle(const sf::Vector2f& vec){
    if(vec.x == 0.0f && vec.y == 0.0f) return 0.0f;
    float absx = std::abs(vec.x), absy = std::abs(vec.y);
    double a = absx > absy ? absy/absx : absx/absy;
    double s = a * a;
    double r = ((-0.0464964749 * s + 0.15931422) * s - 0.327622764) * s * a + a;

    if(absy > absx) r = 1.57079637 - r;
    if(vec.x < 0) r = 3.14159274 - r;
    if(vec.y < 0) r = -r;

    double ang = r*RAD2DEG + 90.0;
    if(ang < 0.0f) ang += 360.0;
    else if(ang > 360.0f) ang -= 360.0;
    return ang;
}

void moveMouse(float amount, float dt) {
    double rotSpeed = amount * dt; //the constant value is in radians/second

    //both camera direction and camera plane must be rotated
    double oldDirX = dirX;
    dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
    dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
    double oldPlaneX = planeX;
    planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
    planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
}

float darkness_distance = 8.0f;
void setBrightness(sf::Vertex& v, float distance, float max_distance) {
    const float max_brightness = 90.0f;
    float darkness = std::max(std::min(max_brightness * (float)distance / (max_distance), max_brightness), 0.0f);
    float brightness = max_brightness - darkness;
    v.color = sf::Color(brightness, brightness, brightness);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(w, h), "SFML window");
    window.setMouseCursorVisible(false);
    window.setMouseCursorGrabbed(true);
    sf::Texture texture;
    texture.setSmooth(true);
    texture.loadFromFile("csgo.png");
    sf::Vector2i ceiling_texture_index{2, 2};
    sf::Vector2i wall_texture_index{2, 0};
    static std::size_t tex_width = 256;
    static std::size_t tex_height = 256;
    sf::Vector2i floor_texture_index{0, 0};
    sf::Vector2f floor_texture_offset{(float)floor_texture_index.x * (tex_width + 2) + 1, (float)floor_texture_index.y * (tex_height + 2) + 1};


    sf::RenderTexture minimap_rt;
    sf::CircleShape minimap_circle(h * 0.125f);
    float minimap_height = minimap_circle.getRadius() * 2;
    minimap_circle.setOrigin(minimap_circle.getRadius(), minimap_circle.getRadius());
    minimap_rt.create(minimap_height, minimap_height);
    minimap_circle.setPosition(10 + minimap_height * 0.5f, h - minimap_height * 0.5f - 10);


    minimap_circle.setOutlineThickness(2);
    float minimap_alpha = 200;
    minimap_circle.setFillColor(sf::Color(255, 255, 255, minimap_alpha));
    minimap_circle.setOutlineColor(sf::Color(0, 0, 0, 200));
    minimap_circle.setTexture(&minimap_rt.getTexture());

    // 2 vertices for a line, 3 lines (ceiling wall floor)
    int vertice_count_per_column = 2;
    sf::VertexArray map_tiles{sf::Quads, mapWidth * mapHeight * 4};
    sf::VertexArray lines(sf::Lines, w * vertice_count_per_column);
    sf::VertexArray points(sf::Points);
    sf::Clock clock;
    sf::Time bobbing_timer;
    sf::Time total_timer;
    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        total_timer += elapsed;

        //std::cout << 1 / elapsed.asSeconds() << std::endl;
        // Process events
        sf::Event event{};
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed ||
                    (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        // Mouse
        {
            sf::Vector2i pos = sf::Mouse::getPosition(window);
            sf::Vector2i center{w / 2, h / 2};
            moveMouse(mouse_sensitivity * (pos.x - center.x), elapsed.asSeconds());
            sf::Mouse::setPosition({w / 2, h / 2}, window);
        }
        //speed modifiers
        double moveSpeed = elapsed.asSeconds() * 5.0; //the constant value is in squares/second

        sf::Vector2f input_dir{(float)(sf::Keyboard::isKeyPressed(sf::Keyboard::D) - (sf::Keyboard::isKeyPressed(sf::Keyboard::A))),
                               (float)(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) -
                                         (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))};

        // If moving diagonally
        if(input_dir.x != 0 && input_dir.y != 0) moveSpeed /= sqrt(2);

        // If moving
        bool moving = input_dir.x != 0 || input_dir.y != 0;
        if(moving) bobbing_timer += elapsed;
        double y_offset = h * (moving ? 0.008 : 0.004) * sin(14.0f * bobbing_timer.asSeconds() + 2.0f * total_timer.asSeconds());

        // Forward
        if (input_dir.y > 0) {
            if (worldMap[int(posX + dirX * moveSpeed)][int(posY)] == 0) posX += dirX * moveSpeed;
            if (worldMap[int(posX)][int(posY + dirY * moveSpeed)] == 0) posY += dirY * moveSpeed;
        }

        if (input_dir.y < 0) {
            if (worldMap[int(posX - dirX * moveSpeed)][int(posY)] == 0) posX -= dirX * moveSpeed;
            if (worldMap[int(posX)][int(posY - dirY * moveSpeed)] == 0) posY -= dirY * moveSpeed;
        }

        if (input_dir.x > 0) {
            sf::Vector2f right_vector{(float)dirY, -(float)dirX};
            if (worldMap[int(posX + right_vector.x * moveSpeed)][int(posY)] == 0) posX += right_vector.x * moveSpeed;
            if (worldMap[int(posX)][int(posY + right_vector.y * moveSpeed)] == 0) posY += right_vector.y * moveSpeed;
        }

        if (input_dir.x < 0) {
            sf::Vector2f right_vector{(float)-dirY, (float)dirX};
            if (worldMap[int(posX + right_vector.x * moveSpeed)][int(posY)] == 0) posX += right_vector.x * moveSpeed;
            if (worldMap[int(posX)][int(posY + right_vector.y * moveSpeed)] == 0) posY += right_vector.y * moveSpeed;
        }

        //rotate to the right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            moveMouse(3, elapsed.asSeconds());
        }
        //rotate to the left
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            moveMouse(-3, elapsed.asSeconds());
        }

        // Update minimap rotation
        minimap_circle.setRotation(90 + vecToAngle({(float)dirX, (float)dirY}));


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
            double lineHeight = std::abs(h / perpWallDist);

            //calculate lowest and highest pixel to fill in current stripe
            double drawStart = y_offset - lineHeight / 2 + h / 2;
            double drawEnd = y_offset + lineHeight / 2 + h / 2;

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

                // Brightness
                float distance = magnitude(sf::Vector2f(mapX - posX + (side == 1 ? wallX : 0), mapY - posY + (side == 0 ? wallX : 0)));
                setBrightness(lines[idx_vx + 0], distance, darkness_distance);
                setBrightness(lines[idx_vx + 1], distance, darkness_distance);
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

            //draw the floor from drawEnd to the bottom of the screen
            for(int y = drawEnd + 1; y < h; y++)
            {
                double currentDist = h / (2.0 * (y - y_offset) - h); //you could make a small lookup table for this instead
                double weight = currentDist / perpWallDist;
                double currentFloorX = weight * floorXWall + (1.0 - weight) * posX;
                double currentFloorY = weight * floorYWall + (1.0 - weight) * posY;

                int floorTexX, floorTexY;
                floorTexX = int(currentFloorX * tex_width) % tex_width;
                floorTexY = int(currentFloorY * tex_height) % tex_height;

                // Prepare floor
                {
                    sf::Vertex vertex({ (float)x - 1, (float)y }, floor_texture_offset + sf::Vector2f{(float)floorTexX, (float)floorTexY});
                    setBrightness(vertex, currentDist, darkness_distance);
                    points.append(vertex);
                }
            }
        }

        // Clear screen
        window.clear(sf::Color::Black);
        window.draw(points, &texture);
        window.draw(lines, &texture);

        // Minimap
        minimap_rt.clear(sf::Color::Black);

        float tile_size = minimap_height / mapWidth;
        sf::Vector2f minimap_pos_offset{-(float)posY * tile_size, -(float)posX * tile_size};
        minimap_pos_offset += 0.5f * sf::Vector2f{mapWidth, mapHeight} * tile_size;
        int idx = 0;
        for(int m_x = 0; m_x < mapWidth; ++m_x) {
            for(int m_y = 0; m_y < mapHeight; ++m_y, idx += 4) {
                int type = worldMap[m_y][m_x];

                auto texture_index = sf::Vector2f{(float)floor_texture_index.x, (float)floor_texture_index.y};

                map_tiles[idx + 0].position = minimap_pos_offset + sf::Vector2f{ m_x * tile_size, m_y * tile_size };
                map_tiles[idx + 1].position = minimap_pos_offset + sf::Vector2f{ (m_x + 1) * tile_size, m_y * tile_size };
                map_tiles[idx + 2].position = minimap_pos_offset + sf::Vector2f{ (m_x + 1) * tile_size, (m_y + 1) * tile_size };
                map_tiles[idx + 3].position = minimap_pos_offset + sf::Vector2f{ m_x * tile_size, (m_y + 1) * tile_size };

                map_tiles[idx + 0].texCoords = { texture_index.x, texture_index.y };
                map_tiles[idx + 1].texCoords = { texture_index.x + tex_width, texture_index.y };
                map_tiles[idx + 2].texCoords = { texture_index.x + tex_width, texture_index.y + tex_height };
                map_tiles[idx + 3].texCoords = { texture_index.x, texture_index.y + tex_height };

                float darkness = 150;
                sf::Color color = type == 0 ? sf::Color::White : sf::Color(darkness, darkness, darkness);

                for(int i = 0; i < 4; ++i) map_tiles[idx + i].color = color;
            }
        }

        minimap_rt.draw(map_tiles, &texture);

        minimap_rt.display();
        window.draw(minimap_circle);


        // FOV in minimap
        sf::VertexArray minimap_fov{sf::Triangles, 3};
        minimap_fov[0].position = minimap_circle.getPosition();
        float fov_rad = (-(fov_degrees * 0.5f) + 90.0f)/RAD2DEG;
        sf::Vector2f fov_vec = sf::Vector2f(cosf(fov_rad), -sinf(fov_rad)) * darkness_distance * tile_size;
        minimap_fov[1].position = minimap_fov[0].position + fov_vec;
        minimap_fov[2].position = minimap_fov[0].position + sf::Vector2f{-fov_vec.x, fov_vec.y};

        sf::Color fov_color = sf::Color(255, 255, 255, 60);
        minimap_fov[0].color = fov_color;

        fov_color.a = 0;
        for(int i = 1; i <= 2; ++i) minimap_fov[i].color = fov_color;
        window.draw(minimap_fov);

        window.display();
    }
    return EXIT_SUCCESS;
}
