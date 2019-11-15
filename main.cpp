#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

// Constants
static const int w = 1920;
static const int h = 1080;
#define mapWidth 24
#define mapHeight 24
static const std::size_t tex_width = 256;
static const std::size_t tex_height = 256;
static const float darkness_distance = 8.0f;
static const double fov_degrees = 103;
static const double fov = fov_degrees / 100.0;
static const float mouse_sensitivity = 0.3f;
static const double minimap_zoom = 0.5;
static const double movement_speed = 5.0;
static const float max_brightness = 90.0f;
static const sf::Vector2i floor_texture_index{0, 0};
static const sf::Vector2i wall_texture_indexes[] = {
        sf::Vector2i{0, 0}, // 0
        sf::Vector2i{2, 0}, // 1
        sf::Vector2i{3, 2}, // 2
        sf::Vector2i{0, 2}, // 3
        sf::Vector2i{1, 0}, // 4
        sf::Vector2i{1, 1}, // 5
};
static int worldMap[mapWidth][mapHeight] = {
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

// Math helpers
namespace {
    // Angle to Vector
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
}

static void setBrightness(sf::Vertex& v, float distance, float max_distance) {
    const float darkness = std::max(std::min(max_brightness * (float)distance / (max_distance), max_brightness), 0.0f);
    const float brightness = max_brightness - darkness;
    v.color = sf::Color(brightness, brightness, brightness);
}

static sf::Vector2f get_texture_offset(const sf::Vector2i& tex_idx) {
    return {(float)tex_idx.x * (tex_width + 2) + 1, (float)tex_idx.y * (tex_height + 2) + 1};
}

static sf::Vector2f get_texture_offset(int type) {
    return get_texture_offset(wall_texture_indexes[type]);
}

int main() {
    // Prepare window
    sf::RenderWindow window(sf::VideoMode(w, h), "Antara Gaming SDK - Wolf3D");
    window.setMouseCursorVisible(false);
    window.setMouseCursorGrabbed(true);

    // Prepare render texture
    sf::RenderTexture rt;
    rt.create(w, h);
    sf::Sprite rt_sprite(rt.getTexture());

    // Wall and floor textures
    sf::Texture texture;
    texture.setSmooth(true);
    texture.loadFromFile("csgo.png");
    sf::Vector2f floor_texture_offset{get_texture_offset(floor_texture_index)};

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

    // Minimap render texture
    const float minimap_height = compass_inner_shadow_texture.getSize().y;

    sf::RenderTexture minimap_rt;
    minimap_rt.create(minimap_height / minimap_zoom, minimap_height / minimap_zoom);
    minimap_rt.setRepeated(false);
    minimap_rt.setSmooth(true);

    // Minimap circle
    sf::CircleShape minimap_circle(minimap_height * 0.5);
    minimap_circle.setTexture(&minimap_rt.getTexture());
    minimap_circle.setOrigin(minimap_circle.getRadius(), minimap_circle.getRadius());
    minimap_circle.setPosition({10 + minimap_height * 0.5f, h - minimap_height * 0.5f - 10});
    minimap_circle.setFillColor(sf::Color(255, 255, 255, 200));

    // COMPASS
    sf::Sprite compass(compass_texture);
    compass.setOrigin(compass_texture.getSize().x / 2, compass_texture.getSize().y / 2);
    compass.setPosition(minimap_circle.getPosition());

    // Compass ring
    sf::Sprite compass_ring(compass_ring_texture);
    compass_ring.setOrigin(compass_ring_texture.getSize().x / 2, compass_ring_texture.getSize().y / 2);
    compass_ring.setColor(sf::Color(70, 70, 70));
    compass_ring.setPosition(minimap_circle.getPosition());

    // Compass inner shadow
    sf::Sprite compass_inner_shadow(compass_inner_shadow_texture);
    compass_inner_shadow.setOrigin(compass_inner_shadow_texture.getSize().x / 2, compass_inner_shadow_texture.getSize().y / 2);
    compass_inner_shadow.setPosition(minimap_circle.getPosition());

    // Compass ring
    sf::Sprite compass_arrow(compass_arrow_texture);
    compass_arrow.setOrigin(compass_arrow_texture.getSize().x / 2, compass_arrow_texture.getSize().y / 2);
    compass_arrow.setScale(1.0f, 1.5f);

    // Arrays
    sf::VertexArray minimap_tiles{sf::Quads, mapWidth * mapHeight * 4};
    sf::VertexArray minimap_fov{sf::Triangles, 3};
    sf::VertexArray wall_lines(sf::Lines, w * 2);
    sf::VertexArray floor_points(sf::Points); 
    
    // Clock and Timer
    sf::Clock clock;
    sf::Time total_timer;

    // Variables
    // Character bobbing timer
    double posX = 22, posY = 12;  // X and Y start position
    double dirX = -1, dirY = 0; // Initial direction vector
    double planeX = 0, planeY = fov; // The 2d ray caster version of camera plane

    sf::Time walking_timer;

    // Game loop
    while(window.isOpen()) {
        sf::Time elapsed = clock.restart();
        total_timer += elapsed;

        // Process events
        {
            sf::Event event{};
            while (window.pollEvent(event)) {
                // Close window: exit
                if (event.type == sf::Event::Closed ||
                    (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                    window.close();
            }
        }

        // Update
        {
            // Camera movement
            {
                // Mouse
                sf::Vector2i pos = sf::Mouse::getPosition(window);
                sf::Vector2i center{w / 2, h / 2};
                sf::Mouse::setPosition(center, window);

                // Move camera
                auto moveCamera = [&dirX, &dirY, &planeX, &planeY](float amount, float dt) {
                    const double rotSpeed = amount * dt; // Constant value is in radians/second

                    // Both camera direction and camera plane must be rotated
                    const double oldDirX = dirX;
                    dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
                    dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
                    const double oldPlaneX = planeX;
                    planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
                    planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
                };

                moveCamera(mouse_sensitivity * (pos.x - center.x), elapsed.asSeconds());

                // Keyboard
                // Rotate to the right
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    moveCamera(3, elapsed.asSeconds());
                }
                // Rotate to the left
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    moveCamera(-3, elapsed.asSeconds());
                }
            }

            // Character movement
            double bobbing_y_offset;
            {
                sf::Vector2i input_dir{(sf::Keyboard::isKeyPressed(sf::Keyboard::D) - (sf::Keyboard::isKeyPressed(sf::Keyboard::A))),
                                       (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) -
                                        (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))};

                // Bobbing
                {
                    bool moving = input_dir.x != 0 || input_dir.y != 0;
                    if(moving) walking_timer += elapsed;
                    bobbing_y_offset = h * (moving ? 0.008 : 0.004) * sin(14.0f * walking_timer.asSeconds() + 2.0f * total_timer.asSeconds());
                }

                // Move forward or back
                {
                    double moveSpeed = elapsed.asSeconds() * movement_speed;

                    // Normalize speed if moving diagonally
                    if(input_dir.x != 0 && input_dir.y != 0) moveSpeed /= sqrt(2);

                    // Move
                    auto move = [&posX, &posY](const sf::Vector2f& vec) {
                        if(worldMap[int(posX + vec.x)][int(posY)] == 0) posX += vec.x;
                        if(worldMap[int(posX)][int(posY + vec.y)] == 0) posY += vec.y;
                    };

                    if(input_dir.y != 0) {
                        auto dir = sf::Vector2f(dirX, dirY) * float(input_dir.y);
                        move(dir * float(moveSpeed));
                    }

                    // Strafe left or right
                    if(input_dir.x != 0) {
                        auto dir = sf::Vector2f{(float)dirY, -(float)dirX} * float(input_dir.x);
                        move(dir * float(moveSpeed));
                    }
                }
            }

            // Update minimap rotation
            {
                minimap_circle.setRotation(vecToAngle({(float)dirX, (float)dirY}));
                compass.setRotation(minimap_circle.getRotation());
            }

            // Prepare wall and floor
            {
                // Clear floor points array
                floor_points.clear();

                // Raycasting algorithm, Loop every vertical line of the screen
                for(int x = 0, idx_vx = 0; x < w; x++, idx_vx += 2) {
            // Calculate ray position and direction
            double cameraX = 2 * x / double(w) - 1; // X-coordinate in camera space

            double rayDirX = dirX + planeX * cameraX;
            double rayDirY = dirY + planeY * cameraX;

            // Length of ray from one x or y-side to next x or y-side
            double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
            double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

            // Calculate step and initial sideDist
            // Length of ray from current position to next x or y-side
            double sideDistX;
            double sideDistY;

            // What direction to step in x or y-direction (either +1 or -1)
            int stepX;
            int stepY;

            // Which box of the map we're in
            int mapX = int(posX);
            int mapY = int(posY);

            // X-direction
            if(rayDirX < 0) {
                stepX = -1;
                sideDistX = (posX - mapX) * deltaDistX;
            }
            else {
                stepX = 1;
                sideDistX = (mapX + 1.0 - posX) * deltaDistX;
            }

            // Y-direction
            if(rayDirY < 0) {
                stepY = -1;
                sideDistY = (posY - mapY) * deltaDistY;
            }
            else {
                stepY = 1;
                sideDistY = (mapY + 1.0 - posY) * deltaDistY;
            }

            // Perform DDA
            int side; // Was a NS or a EW wall hit?
            int hit = 0; // Was there a wall hit?
            double perpWallDist;

            while(hit == 0) {
                // Jump to next map square, OR in X-direction, OR in Y-direction
                if(sideDistX < sideDistY) {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                }
                else {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }

                // Check if ray has hit a wall
                if(worldMap[mapX][mapY] > 0) hit = 1;
            }

            // Calculate distance projected on camera direction (Euclidean distance will give fish-eye effect!)
            if(side == 0) {
                perpWallDist = std::fabs((mapX - posX + (1 - stepX) / 2) / rayDirX);
            }
            else {
                perpWallDist = std::fabs((mapY - posY + (1 - stepY) / 2) / rayDirY);
            }

            // Calculate height of line to draw on screen
            double lineHeight = std::abs(h / perpWallDist);

            // Calculate lowest and highest pixel to fill in current stripe
            double drawStart = bobbing_y_offset - lineHeight * 0.5 + h * 0.5;
            double drawEnd = bobbing_y_offset + lineHeight * 0.5 + h * 0.5;

            // Calculate value of wallX
            double wallX; // Where exactly the wall was hit
            if(side == 0) wallX = posY + perpWallDist * rayDirY;
            else wallX = posX + perpWallDist * rayDirX;
            wallX -= floor((wallX));

            // X coordinate on the texture
            int texX = int(wallX * double(tex_width));
            if(side == 0 && rayDirX > 0) texX = tex_width - texX - 1;
            if(side == 1 && rayDirY < 0) texX = tex_height - texX - 1;

            // Prepare wall line
            {
                auto offset = get_texture_offset(worldMap[mapX][mapY]);
                wall_lines[idx_vx + 0].texCoords = offset + sf::Vector2f{float(texX), 0};
                wall_lines[idx_vx + 1].texCoords = offset + sf::Vector2f{float(texX), (float)tex_height};
                wall_lines[idx_vx + 0].position = {float(x), (float) drawStart};
                wall_lines[idx_vx + 1].position = {float(x), (float) drawEnd};

                // Brightness
                float distance = magnitude(sf::Vector2f(mapX - posX + (side == 1 ? wallX : 0), mapY - posY + (side == 0 ? wallX : 0)));
                setBrightness(wall_lines[idx_vx + 0], distance, darkness_distance);
                setBrightness(wall_lines[idx_vx + 1], distance, darkness_distance);
            }


            // FLOOR CASTING
            double floorXWall, floorYWall; // X, Y position of the floor texel at the bottom of the wall

            // Four different wall directions possible
            if(side == 0 && rayDirX > 0) {
                floorXWall = mapX;
                floorYWall = mapY + wallX;
            }
            else if(side == 0 && rayDirX < 0) {
                floorXWall = mapX + 1.0;
                floorYWall = mapY + wallX;
            }
            else if(side == 1 && rayDirY > 0) {
                floorXWall = mapX + wallX;
                floorYWall = mapY;
            }
            else {
                floorXWall = mapX + wallX;
                floorYWall = mapY + 1.0;
            }

            // Draw the floor from drawEnd to the bottom of the screen
            for(int y = drawEnd + 1; y < h; y++)
            {
                double currentDist = h / (2.0 * (y - bobbing_y_offset) - h); //you could make a small lookup table for this instead
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
                    floor_points.append(vertex);
                }
            }
        }
            }
        }

        // Render
        {
            // Clear window and render texture
            {
                window.clear(sf::Color::Black);
                rt.clear(sf::Color::Black);
            }

            // Draw everything
            {
                // Draw wall and floor
                {
                    rt.draw(wall_lines, &texture);
                    rt.draw(floor_points, &texture);
                }

                // Minimap
                {
                    // Clear render texture
                    {
                        minimap_rt.clear(sf::Color::Black);
                    }

                    // Tiles
                    const float tile_size = (float)minimap_rt.getSize().y / mapWidth;
                    {
                        for(int m_x = 0, idx = 0; m_x < mapWidth; ++m_x) {
                            for(int m_y = 0; m_y < mapHeight; ++m_y, idx += 4) {
                                const int type = worldMap[m_y][m_x];

                                // Position
                                minimap_tiles[idx + 0].position = sf::Vector2f{ m_x * tile_size, m_y * tile_size };
                                minimap_tiles[idx + 1].position = sf::Vector2f{ (m_x + 1) * tile_size, m_y * tile_size };
                                minimap_tiles[idx + 2].position = sf::Vector2f{ (m_x + 1) * tile_size, (m_y + 1) * tile_size };
                                minimap_tiles[idx + 3].position = sf::Vector2f{ m_x * tile_size, (m_y + 1) * tile_size };

                                // Texture
                                const auto offset = get_texture_offset(type);
                                minimap_tiles[idx + 0].texCoords = { offset.x, offset.y };
                                minimap_tiles[idx + 1].texCoords = { offset.x + tex_width, offset.y };
                                minimap_tiles[idx + 2].texCoords = { offset.x + tex_width, offset.y + tex_height };
                                minimap_tiles[idx + 3].texCoords = { offset.x, offset.y + tex_height };

                                // Color
                                const float darkness = 150;
                                sf::Color color = type == 0 ? sf::Color(darkness, darkness, darkness) : sf::Color::White;
                                for(int i = 0; i < 4; ++i) minimap_tiles[idx + i].color = color;
                            }
                        }

                        // Draw to minimap render texture
                        minimap_rt.draw(minimap_tiles, &texture);
                    }

                    // Draw FOV in minimap
                    const sf::Vector2f character_pos{(float)posY * tile_size, (float)posX * tile_size};
                    const float character_dir_angle = -vecToAngle({(float)dirX, (float)dirY});
                    {
                        // Angles
                        const float fov_arm_dist = tile_size * darkness_distance * (minimap_height / minimap_rt.getSize().x) / minimap_zoom;
                        const sf::Vector2f left_fov_vec = angleToVec(character_dir_angle - fov_degrees * 0.5f) * fov_arm_dist;
                        const sf::Vector2f right_fov_vec = angleToVec(character_dir_angle + fov_degrees * 0.5f) * fov_arm_dist;

                        // Positions
                        minimap_fov[0].position = character_pos;
                        minimap_fov[1].position = minimap_fov[0].position + left_fov_vec;
                        minimap_fov[2].position = minimap_fov[0].position + right_fov_vec;

                        // Character point is visible
                        sf::Color fov_color = sf::Color(255, 255, 255, 60);
                        minimap_fov[0].color = fov_color;

                        // Then it goes invisible towards the end
                        fov_color.a = 0;
                        for(int i = 1; i <= 2; ++i) minimap_fov[i].color = fov_color;

                        // Draw to minimap render texture
                        minimap_rt.draw(minimap_fov);
                    }

                    // Draw compass character arrow
                    {
                        compass_arrow.setPosition(character_pos);
                        compass_arrow.setRotation(character_dir_angle);
                        minimap_rt.draw(compass_arrow);
                    }

                    // Render minimap circle
                    {
                        // Finalize minimap render texture
                        minimap_rt.display();
                        const sf::Vector2f minimap_rt_size{(float)minimap_rt.getSize().x, (float)minimap_rt.getSize().y};
                        const sf::Vector2f minimap_pos_offset = tile_size * -(0.5f * sf::Vector2f{mapWidth, mapHeight} + sf::Vector2f{-(float)posY, -(float)posX});
                        const sf::Vector2i minimap_rect_size{static_cast<int>(minimap_zoom * minimap_rt_size.x), static_cast<int>(minimap_zoom * minimap_rt_size.y)};

                        // TODO: Character is not centered at borders. Instead of limiting the edges, we can have a bigger RenderTexture so minimap won't ever reach to the edges
                        minimap_circle.setTextureRect({std::min(std::max(static_cast<int>(minimap_pos_offset.x + (1 - minimap_zoom) * 0.5f * minimap_rt_size.x), 0), static_cast<int>(minimap_rt_size.x - minimap_rect_size.x)),
                                                              std::min(std::max(static_cast<int>(minimap_pos_offset.y + (1 - minimap_zoom) * 0.5f * minimap_rt_size.y), 0), static_cast<int>(minimap_rt_size.y - minimap_rect_size.y)),
                                                              minimap_rect_size.x, minimap_rect_size.y});
                        rt.draw(minimap_circle);
                    }

                    // Render compass
                    {
                        rt.draw(compass_inner_shadow);
                        rt.draw(compass);
                        rt.draw(compass_ring);
                    }
                }
            }

            // Finalize render texture, draw it to the window, then finalize the window rendering
            {
                rt.display();
                window.draw(rt_sprite);
                window.display();
            }

            // Print FPS
            {
                //std::cout << 1 / elapsed.asSeconds() << std::endl;
            }
        }
    }

    return EXIT_SUCCESS;
}
