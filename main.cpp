#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#define mapWidth 24
#define mapHeight 24

static sf::Vector2i wall_texture_indexes[] = {
    sf::Vector2i{0, 0}, // 0
    sf::Vector2i{2, 0}, // 1
    sf::Vector2i{3, 2}, // 2
    sf::Vector2i{0, 2}, // 3
    sf::Vector2i{1, 0}, // 4
    sf::Vector2i{1, 1}, // 5
};


static const std::size_t tex_width = 256;
static const std::size_t tex_height = 256;
sf::Vector2f get_texture_offset(int type) {
    auto tex_idx = wall_texture_indexes[type];

    return {(float)tex_idx.x * (tex_width + 2) + 1, (float)tex_idx.y * (tex_height + 2) + 1};
}

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


static const sf::Vector2f UNIT_UP(0, -1);
static const sf::Vector2f UNIT_UP_RIGHT(1, -1);
static const sf::Vector2f UNIT_UP_LEFT(-1, -1);
static const sf::Vector2f UNIT_DOWN(0, 1);
static const sf::Vector2f UNIT_DOWN_RIGHT(1, 1);
static const sf::Vector2f UNIT_DOWN_LEFT(-1, 1);
static const sf::Vector2f UNIT_RIGHT(1, 0);
static const sf::Vector2f UNIT_LEFT(-1, 0);
constexpr float RAD2DEG = 57.295779513082320876798154814105f;
static sf::Vector2f angleToVec(const float& degree) {
    if(degree == 180) return UNIT_DOWN;
    if(degree == 90) return UNIT_RIGHT;
    if(degree == 270) return UNIT_LEFT;
    if(degree == 0 || degree == 360) return UNIT_UP;
    if(degree == 45) return UNIT_UP_RIGHT;
    if(degree == 135) return UNIT_DOWN_RIGHT;
    if(degree == 225) return UNIT_DOWN_LEFT;
    if(degree == 315) return UNIT_UP_LEFT;

    float rad = (-degree + 90.0f)/RAD2DEG;
    return sf::Vector2f(cosf(rad), -sinf(rad));
}

static double posX = 22, posY = 12;  //x and y start position
static double dirX = -1, dirY = 0; //initial direction vector
static double fov_degrees = 103;
static double fov = fov_degrees / 100.0;
static double planeX = 0, planeY = fov; //the 2d raycaster version of camera plane

static const int w = 1920;
static const int h = 1080;
static const float mouse_sensitivity = 0.3f;

static float magnitude(const sf::Vector2f& a) {
    return sqrt(a.x * a.x + a.y * a.y);
}

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
    return ang + 90;
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
    sf::RenderTexture rt;
    rt.create(w, h);
    sf::Sprite rt_sprite(rt.getTexture());

    window.setMouseCursorVisible(false);
    window.setMouseCursorGrabbed(true);
    sf::Texture texture;
    texture.setSmooth(true);
    texture.loadFromFile("csgo.png");
    sf::Vector2i floor_texture_index{0, 0};
    sf::Vector2f floor_texture_offset{(float)floor_texture_index.x * (tex_width + 2) + 1, (float)floor_texture_index.y * (tex_height + 2) + 1};

    // Compass Textures
    sf::Texture compass_texture;
    compass_texture.setSmooth(true);
    compass_texture.loadFromFile("compass.png");
    sf::Texture compass_inner_shadow_texture;
    compass_inner_shadow_texture.setSmooth(true);
    compass_inner_shadow_texture.loadFromFile("compass_inner_shadow.png");
    sf::Texture compass_ring_texture;
    compass_ring_texture.setSmooth(true);
    compass_ring_texture.loadFromFile("compass_ring.png");
    sf::Texture compass_arrow_texture;
    compass_arrow_texture.setSmooth(true);
    compass_arrow_texture.loadFromFile("compass_arrow.png");

    sf::RenderTexture minimap_rt;
    sf::CircleShape minimap_circle(compass_inner_shadow_texture.getSize().y / 2);
    float minimap_height = minimap_circle.getRadius() * 2;
    minimap_circle.setOrigin(minimap_circle.getRadius(), minimap_circle.getRadius());
    float minimap_zoom = 0.5f;
    minimap_rt.create(minimap_height / minimap_zoom, minimap_height / minimap_zoom);
    minimap_rt.setRepeated(false);
    minimap_rt.setSmooth(true);
    minimap_circle.setPosition(10 + minimap_height * 0.5f, h - minimap_height * 0.5f - 10);


    float minimap_alpha = 200;
    minimap_circle.setFillColor(sf::Color(255, 255, 255, minimap_alpha));
    minimap_circle.setTexture(&minimap_rt.getTexture());

    // COMPASS
    sf::Sprite compass(compass_texture);
    compass.setOrigin(compass_texture.getSize().x / 2, compass_texture.getSize().y / 2);
    compass.setPosition(minimap_circle.getPosition());

    // Compass ring
    sf::Sprite compass_ring(compass_ring_texture);
    compass_ring.setOrigin(compass_ring_texture.getSize().x / 2, compass_ring_texture.getSize().y / 2);
    compass_ring.setPosition(minimap_circle.getPosition());
    compass_ring.setColor(sf::Color(70, 70, 70));

    // Compass inner shadow
    sf::Sprite compass_inner_shadow(compass_inner_shadow_texture);
    compass_inner_shadow.setOrigin(compass_inner_shadow_texture.getSize().x / 2, compass_inner_shadow_texture.getSize().y / 2);
    compass_inner_shadow.setPosition(minimap_circle.getPosition());

    // Compass ring
    sf::Sprite compass_arrow(compass_arrow_texture);
    compass_arrow.setOrigin(compass_arrow_texture.getSize().x / 2, compass_arrow_texture.getSize().y / 2);
    compass_arrow.setScale(1.0f, 1.5f);

    // 2 vertices for a line, 3 lines (ceiling wall floor)
    int vertice_count_per_column = 2;
    sf::VertexArray map_tiles{sf::Quads, mapWidth * mapHeight * 4};
    sf::VertexArray lines(sf::Lines, w * vertice_count_per_column);
    sf::VertexArray points(sf::Points);
    sf::Clock clock;
    sf::Time bobbing_timer;
    sf::Time total_timer;
    while(window.isOpen()) {
        sf::Time elapsed = clock.restart();
        total_timer += elapsed;

        //std::cout << 1 / elapsed.asSeconds() << std::endl;
        // Process events
        sf::Event event{};
        while(window.pollEvent(event)) {
            // Close window: exit
            if(event.type == sf::Event::Closed ||
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

        sf::Vector2i input_dir{(sf::Keyboard::isKeyPressed(sf::Keyboard::D) - (sf::Keyboard::isKeyPressed(sf::Keyboard::A))),
                               (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) -
                                (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))};

        // If moving diagonally
        if(input_dir.x != 0 && input_dir.y != 0) moveSpeed /= sqrt(2);

        // If moving
        bool moving = input_dir.x != 0 || input_dir.y != 0;
        if(moving) bobbing_timer += elapsed;
        double y_offset = h * (moving ? 0.008 : 0.004) * sin(14.0f * bobbing_timer.asSeconds() + 2.0f * total_timer.asSeconds());

        // Forward
        if(input_dir.y > 0) {
            if(worldMap[int(posX + dirX * moveSpeed)][int(posY)] == 0) posX += dirX * moveSpeed;
            if(worldMap[int(posX)][int(posY + dirY * moveSpeed)] == 0) posY += dirY * moveSpeed;
        }

        if(input_dir.y < 0) {
            if(worldMap[int(posX - dirX * moveSpeed)][int(posY)] == 0) posX -= dirX * moveSpeed;
            if(worldMap[int(posX)][int(posY - dirY * moveSpeed)] == 0) posY -= dirY * moveSpeed;
        }

        if(input_dir.x > 0) {
            sf::Vector2f right_vector{(float)dirY, -(float)dirX};
            if(worldMap[int(posX + right_vector.x * moveSpeed)][int(posY)] == 0) posX += right_vector.x * moveSpeed;
            if(worldMap[int(posX)][int(posY + right_vector.y * moveSpeed)] == 0) posY += right_vector.y * moveSpeed;
        }

        if(input_dir.x < 0) {
            sf::Vector2f right_vector{(float)-dirY, (float)dirX};
            if(worldMap[int(posX + right_vector.x * moveSpeed)][int(posY)] == 0) posX += right_vector.x * moveSpeed;
            if(worldMap[int(posX)][int(posY + right_vector.y * moveSpeed)] == 0) posY += right_vector.y * moveSpeed;
        }

        //rotate to the right
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            moveMouse(3, elapsed.asSeconds());
        }
        //rotate to the left
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            moveMouse(-3, elapsed.asSeconds());
        }

        // Update minimap rotation
        minimap_circle.setRotation(vecToAngle({(float)dirX, (float)dirY}));
        compass.setRotation(minimap_circle.getRotation());


        points.clear();
        for(int x = 0, idx_vx = 0; x < w; x++, idx_vx += vertice_count_per_column) {
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
            double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
            double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

            double perpWallDist;

            //what direction to step in x or y-direction (either +1 or -1)
            int stepX;
            int stepY;

            int hit = 0; //was there a wall hit?
            int side; //was a NS or a EW wall hit?
            //calculate step and initial sideDist
            if(rayDirX < 0) {
                stepX = -1;
                sideDistX = (posX - mapX) * deltaDistX;
            } else {
                stepX = 1;
                sideDistX = (mapX + 1.0 - posX) * deltaDistX;
            }
            if(rayDirY < 0) {
                stepY = -1;
                sideDistY = (posY - mapY) * deltaDistY;
            } else {
                stepY = 1;
                sideDistY = (mapY + 1.0 - posY) * deltaDistY;
            }
            //perform DDA
            while(hit == 0) {
                //jump to next map square, OR in x-direction, OR in y-direction
                if(sideDistX < sideDistY) {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                } else {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }
                //Check if ray has hit a wall
                if(worldMap[mapX][mapY] > 0) hit = 1;
            }
            //Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
            if(side == 0) {
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
            if(side == 0) wallX = posY + perpWallDist * rayDirY;
            else wallX = posX + perpWallDist * rayDirX;
            wallX -= floor((wallX));

            //x coordinate on the texture
            int texX = int(wallX * double(tex_width));
            if(side == 0 && rayDirX > 0) texX = tex_width - texX - 1;
            if(side == 1 && rayDirY < 0) texX = tex_height - texX - 1;


            // Prepare wall line
            {
                auto offset = get_texture_offset(worldMap[mapX][mapY]);
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
        rt.clear(sf::Color::Black);
        rt.draw(points, &texture);
        rt.draw(lines, &texture);

        // Minimap
        minimap_rt.clear(sf::Color::Black);

        float tile_size = (float)minimap_rt.getSize().y / mapWidth;
        int idx = 0;
        for(int m_x = 0; m_x < mapWidth; ++m_x) {
            for(int m_y = 0; m_y < mapHeight; ++m_y, idx += 4) {
                int type = worldMap[m_y][m_x];
                auto offset = get_texture_offset(worldMap[m_y][m_x]);

                map_tiles[idx + 0].position = sf::Vector2f{ m_x * tile_size, m_y * tile_size };
                map_tiles[idx + 1].position = sf::Vector2f{ (m_x + 1) * tile_size, m_y * tile_size };
                map_tiles[idx + 2].position = sf::Vector2f{ (m_x + 1) * tile_size, (m_y + 1) * tile_size };
                map_tiles[idx + 3].position = sf::Vector2f{ m_x * tile_size, (m_y + 1) * tile_size };

                map_tiles[idx + 0].texCoords = { offset.x, offset.y };
                map_tiles[idx + 1].texCoords = { offset.x + tex_width, offset.y };
                map_tiles[idx + 2].texCoords = { offset.x + tex_width, offset.y + tex_height };
                map_tiles[idx + 3].texCoords = { offset.x, offset.y + tex_height };

                float darkness = 150;
                sf::Color color = type == 0 ? sf::Color(darkness, darkness, darkness) : sf::Color::White;

                for(int i = 0; i < 4; ++i) {
                    map_tiles[idx + i].color = color;
                }
            }
        }


        sf::Vector2f minimap_rt_size{(float)minimap_rt.getSize().x, (float)minimap_rt.getSize().y};

        sf::Vector2f minimap_pos_offset{-(float)posY * tile_size, -(float)posX * tile_size};
        minimap_pos_offset += 0.5f * sf::Vector2f{mapWidth, mapHeight} * tile_size;
        minimap_pos_offset *= -1.0f;

        sf::Vector2i minimap_rect_size{static_cast<int>(minimap_zoom * minimap_rt_size.x), static_cast<int>(minimap_zoom * minimap_rt_size.y)};
        // TODO: Character is not centered at borders. Instead of limiting the edges, we can have a bigger RenderTexture so minimap won't ever reach to the edges
        minimap_circle.setTextureRect({
                std::min(std::max(static_cast<int>(minimap_pos_offset.x + (1 - minimap_zoom) * 0.5f * minimap_rt_size.x), 0),
                         static_cast<int>(minimap_rt_size.x - minimap_rect_size.x)),
                         std::min(std::max(static_cast<int>(minimap_pos_offset.y + (1 - minimap_zoom) * 0.5f * minimap_rt_size.y), 0), static_cast<int>(minimap_rt_size.y - minimap_rect_size.y)),
                                  minimap_rect_size.x, minimap_rect_size.y});

        minimap_rt.draw(map_tiles, &texture);

        // FOV in minimap
        sf::VertexArray minimap_fov{sf::Triangles, 3};
        float fov_arm_dist = tile_size * darkness_distance * (minimap_height / minimap_rt.getSize().x) / minimap_zoom;
        float dir_angle = -vecToAngle({(float)dirX, (float)dirY});
        sf::Vector2f left_fov_vec = angleToVec(dir_angle - fov_degrees * 0.5f) * fov_arm_dist;
        sf::Vector2f right_fov_vec = angleToVec(dir_angle + fov_degrees * 0.5f) * fov_arm_dist;

        minimap_fov[0].position = sf::Vector2f{(float)posY * tile_size, (float)posX * tile_size};

        minimap_fov[1].position = minimap_fov[0].position + left_fov_vec;
        minimap_fov[2].position = minimap_fov[0].position + right_fov_vec;

        sf::Color fov_color = sf::Color(255, 255, 255, 60);
        minimap_fov[0].color = fov_color;

        fov_color.a = 0;
        for(int i = 1; i <= 2; ++i) minimap_fov[i].color = fov_color;
        minimap_rt.draw(minimap_fov);

        // Compass Character arrow
        compass_arrow.setPosition(minimap_fov[0].position);
        compass_arrow.setRotation(dir_angle);
        minimap_rt.draw(compass_arrow);

        // Display minimap
        minimap_rt.display();
        rt.draw(minimap_circle);

        // Compass
        rt.draw(compass_inner_shadow);
        rt.draw(compass);
        rt.draw(compass_ring);

        rt.display();
        window.draw(rt_sprite);
        window.display();
    }
    return EXIT_SUCCESS;
}
