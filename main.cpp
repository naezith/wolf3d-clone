#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <chrono>

using namespace std::chrono_literals;

// Constants
static const int w = 1440;
static const int h = 900;
#define mapWidth 24
#define mapHeight 24
static const std::size_t tex_width = 256;
static const std::size_t tex_height = 256;
static const float darkness_distance = 8.0f;
static const float fov_degrees = 103;
static const float fov = fov_degrees / 100.0f;
static const float mouse_sensitivity = 0.15f;
static const float minimap_zoom = 0.5f;
static const float movement_speed = 5.0f;
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
static const int worldMap[mapWidth][mapHeight] = {
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
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 1},
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

        const float rad = (-degree + 90.0f)/RAD2DEG;
        return sf::Vector2f(cosf(rad), -sinf(rad));
    }

    static float magnitude(const sf::Vector2f& a) {
        return sqrt(a.x * a.x + a.y * a.y);
    }

    static float vecToAngle(const sf::Vector2f& vec){
        if(vec.x == 0.0f && vec.y == 0.0f) return 0.0f;
        const float absx = std::abs(vec.x), absy = std::abs(vec.y);
        const float a = absx > absy ? absy/absx : absx/absy;
        const float s = a * a;
        float r = ((-0.0464964749 * s + 0.15931422) * s - 0.327622764) * s * a + a;

        if(absy > absx) r = 1.57079637 - r;
        if(vec.x < 0) r = 3.14159274 - r;
        if(vec.y < 0) r = -r;

        float ang = r*RAD2DEG + 90.0f;
        if(ang < 0.0f) ang += 360.0f;
        else if(ang > 360.0f) ang -= 360.0f;
        return ang + 90;
    }

    static void setColor(sf::Vertex& v, unsigned char value) {
        v.color.r = v.color.g = v.color.b = value;
    }

    static void setColor(sf::Vertex& v, unsigned char value, unsigned char alpha) {
        setColor(v, value);
        v.color.a = alpha;
    }
}

static void setBrightness(sf::Vertex& v, const float distance, const float max_distance) {
    const float darkness = std::max(std::min(max_brightness * distance / max_distance, max_brightness), 0.0f);
    const float brightness = max_brightness - darkness;
    setColor(v, brightness);
}

static sf::Vector2f get_texture_offset(const sf::Vector2i& tex_idx) {
    return sf::Vector2f(tex_idx.x * (tex_width + 2) + 1, tex_idx.y * (tex_height + 2) + 1);
}

static sf::Vector2f get_texture_offset(const int type) {
    return get_texture_offset(wall_texture_indexes[type]);
}

int main() {
    // Prepare window
    sf::RenderWindow window(sf::VideoMode(w, h), "Antara Gaming SDK - Wolf3D");
    window.setMouseCursorVisible(false);
    window.setMouseCursorGrabbed(true);

    // Vertical sync, might wanna disable
    window.setVerticalSyncEnabled(true);

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

    // Minimap tile size
    const float tile_size = static_cast<float>(minimap_rt.getSize().y) / mapWidth;

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
    sf::Vertex floor_vertex;
    
    // Clock and Timer
    sf::Clock clock;
    sf::Text fps_text;
    sf::Font font;
    font.loadFromFile("Ubuntu-L.ttf");
    fps_text.setFont(font);

    // Variables
    float posX = 22, posY = 12;  // X and Y start position
    float dirX = -1, dirY = 0; // Initial direction vector
    float planeX = 0, planeY = fov; // The 2d ray caster version of camera plane

    // Move camera
    auto moveCamera = [&dirX, &dirY, &planeX, &planeY](const float amount, const float dt) {
        const float rotSpeed = amount * dt; // Constant value is in radians/second

        // Both camera direction and camera plane must be rotated
        const float oldDirX = dirX;
        dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
        dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
        const float oldPlaneX = planeX;
        planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
        planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    };

    // Character bobbing timer
    float bobbing_y_offset;
    float walking_timer = 0.0f;

    // Game loop
    const float dt = 0.008f;
    std::chrono::nanoseconds timestep(8ms), lag(0ns);
    auto time_start = std::chrono::high_resolution_clock::now();
    float total_timer = 0.0f;

    // FPS counter
    const float fps_average_every_seconds = 1.0f;
    float fps_time_sum = 0.0f;
    int fps_capture_count = 0;

    while(window.isOpen()) {
        // Calculate the elapsed time and add it to lag
        auto now = std::chrono::high_resolution_clock::now();
        lag += now - time_start;
        time_start = now;

        // FPS Counter
        float elapsed_time = clock.restart().asSeconds();
        fps_time_sum += elapsed_time;
        ++fps_capture_count;
        if(fps_time_sum > fps_average_every_seconds) {
            // Calculate average rounded fps
            const float avg_fps = std::round(1 / (fps_time_sum / fps_capture_count));

            // Reset
            fps_time_sum = 0.0f;
            fps_capture_count = 0;

            // Set string
            std::string fps_str = std::to_string(avg_fps);
            fps_str.erase(fps_str.find_last_not_of('0'), std::string::npos);
            fps_text.setString(fps_str);
        }

        // Update loop
        while(lag >= timestep) {
            //std::cout << "Update: " << lag.count() << std::endl;
            lag -= timestep;
            total_timer += dt;

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

            // Update everything
            {
                // Camera movement
                {
                    // Mouse
                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                    sf::Vector2i center{w / 2, h / 2};
                    sf::Mouse::setPosition(center, window);

                    moveCamera(mouse_sensitivity * (pos.x - center.x), dt);

                    // Keyboard
                    float input_dir = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) - sf::Keyboard::isKeyPressed(sf::Keyboard::Left);

                    if(input_dir != 0) moveCamera(input_dir * 3, dt);
                }

                // Character movement
                {
                    sf::Vector2f input_dir((sf::Keyboard::isKeyPressed(sf::Keyboard::D) - (sf::Keyboard::isKeyPressed(sf::Keyboard::A))),
                                           (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) -
                                            (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)));

                    // Bobbing
                    {
                        bool moving = input_dir.x != 0 || input_dir.y != 0;
                        if(moving) walking_timer += dt;
                        bobbing_y_offset = h * (moving ? 0.008 : 0.004) * sin(14.0f * walking_timer + 2.0f * total_timer);
                    }

                    // Move forward or back
                    {
                        // Set current movement speed, delta
                        float moveSpeed = dt * movement_speed;

                        // Normalize speed if moving diagonally
                        if(input_dir.x != 0 && input_dir.y != 0) moveSpeed /= sqrt(2);

                        // Move
                        auto move = [&posX, &posY](const sf::Vector2f& vec) {
                            if(worldMap[static_cast<int>(posX + vec.x)][static_cast<int>(posY)] == 0) posX += vec.x;
                            if(worldMap[static_cast<int>(posX)][static_cast<int>(posY + vec.y)] == 0) posY += vec.y;
                        };

                        if(input_dir.y != 0) {
                            auto dir = sf::Vector2f(dirX, dirY) * input_dir.y;
                            move(dir * moveSpeed);
                        }

                        // Strafe left or right
                        if(input_dir.x != 0) {
                            auto dir = sf::Vector2f{dirY, -dirX} * input_dir.x;
                            move(dir * moveSpeed);
                        }
                    }
                }

                // Update minimap rotation
                {
                    minimap_circle.setRotation(vecToAngle({dirX, dirY}));
                    compass.setRotation(minimap_circle.getRotation());
                }
            }
        }

        // Prepare render
        {
            // Wall and floor
            {
                // Clear floor points array
                floor_points.clear();

                // Raycasting algorithm, Loop every vertical line of the screen
                for(int x = 0, idx_vx = 0; x < w; ++x, idx_vx += 2) {
                    // Calculate ray position and direction
                    const float cameraX = 2.0f * x / w - 1; // X-coordinate in camera space

                    const float rayDirX = dirX + planeX * cameraX;
                    const float rayDirY = dirY + planeY * cameraX;

                    // Length of ray from one x or y-side to next x or y-side
                    const float deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
                    const float deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

                    // Calculate step and initial sideDist
                    // Length of ray from current position to next x or y-side
                    float sideDistX;
                    float sideDistY;

                    // What direction to step in x or y-direction (either +1 or -1)
                    int stepX;
                    int stepY;

                    // Which box of the map we're in
                    int mapX = posX;
                    int mapY = posY;

                    // X-direction
                    if(rayDirX < 0) {
                        stepX = -1;
                        sideDistX = (posX - mapX) * deltaDistX;
                    }
                    else {
                        stepX = 1;
                        sideDistX = (mapX + 1.0f - posX) * deltaDistX;
                    }

                    // Y-direction
                    if(rayDirY < 0) {
                        stepY = -1;
                        sideDistY = (posY - mapY) * deltaDistY;
                    }
                    else {
                        stepY = 1;
                        sideDistY = (mapY + 1.0f - posY) * deltaDistY;
                    }

                    // Perform DDA
                    int side; // Was a NS or a EW wall hit?
                    int hit = 0; // Was there a wall hit?
                    float perpWallDist;

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
                    const float lineHeight = std::abs(h / perpWallDist);

                    // Calculate lowest and highest pixel to fill in current stripe
                    const float drawStart = bobbing_y_offset - lineHeight * 0.5f + h * 0.5f;
                    const float drawEnd = bobbing_y_offset + lineHeight * 0.5f + h * 0.5f;

                    // Calculate value of wallX
                    float wallX; // Where exactly the wall was hit
                    if(side == 0) wallX = posY + perpWallDist * rayDirY;
                    else wallX = posX + perpWallDist * rayDirX;
                    wallX -= floor((wallX));

                    // X coordinate on the texture
                    int texX = wallX * tex_width;
                    if(side == 0 && rayDirX > 0) texX = tex_width - texX - 1;
                    if(side == 1 && rayDirY < 0) texX = tex_height - texX - 1;

                    // Prepare wall line
                    {
                        auto offset = get_texture_offset(worldMap[mapX][mapY]);
                        wall_lines[idx_vx + 0].texCoords = offset + sf::Vector2f(texX, 0);
                        wall_lines[idx_vx + 1].texCoords = offset + sf::Vector2f(texX, tex_height);
                        wall_lines[idx_vx + 0].position = sf::Vector2f(x, drawStart);
                        wall_lines[idx_vx + 1].position = sf::Vector2f(x, drawEnd);

                        // Brightness
                        const float distance = magnitude(sf::Vector2f(mapX - posX + (side == 1 ? wallX : 0), mapY - posY + (side == 0 ? wallX : 0)));
                        setBrightness(wall_lines[idx_vx + 0], distance, darkness_distance);
                        setBrightness(wall_lines[idx_vx + 1], distance, darkness_distance);
                    }


                    // FLOOR CASTING
                    float floorXWall, floorYWall; // X, Y position of the floor texel at the bottom of the wall

                    // Four different wall directions possible
                    if(side == 0 && rayDirX > 0) {
                        floorXWall = mapX;
                        floorYWall = mapY + wallX;
                    }
                    else if(side == 0 && rayDirX < 0) {
                        floorXWall = mapX + 1.0f;
                        floorYWall = mapY + wallX;
                    }
                    else if(side == 1 && rayDirY > 0) {
                        floorXWall = mapX + wallX;
                        floorYWall = mapY;
                    }
                    else {
                        floorXWall = mapX + wallX;
                        floorYWall = mapY + 1.0f;
                    }

                    // Draw the floor from drawEnd to the bottom of the screen
                    for(int y = drawEnd + 1; y < h; ++y) {
                        const float currentDist = h / (2.0f * (y - bobbing_y_offset) - h); //you could make a small lookup table for this instead
                        const float weight = currentDist / perpWallDist;
                        const float currentFloorX = weight * floorXWall + (1.0f - weight) * posX;
                        const float currentFloorY = weight * floorYWall + (1.0f - weight) * posY;

                        const int floorTexX = static_cast<int>(currentFloorX * tex_width) % tex_width;
                        const int floorTexY = static_cast<int>(currentFloorY * tex_height) % tex_height;

                        // Prepare floor
                        {
                            floor_vertex.position = sf::Vector2f(x - 1, y);
                            floor_vertex.texCoords = floor_texture_offset + sf::Vector2f(floorTexX, floorTexY);
                            setBrightness(floor_vertex, currentDist, darkness_distance);
                            floor_points.append(floor_vertex);
                        }
                    }
                }
            }

            // Minimap
            {
                // Tiles
                {
                    for(int m_x = 0, idx = 0; m_x < mapWidth; ++m_x) {
                        for(int m_y = 0; m_y < mapHeight; ++m_y, idx += 4) {
                            const int type = worldMap[m_y][m_x];

                            // Position
                            minimap_tiles[idx + 0].position = { m_x * tile_size, m_y * tile_size };
                            minimap_tiles[idx + 1].position = { (m_x + 1) * tile_size, m_y * tile_size };
                            minimap_tiles[idx + 2].position = { (m_x + 1) * tile_size, (m_y + 1) * tile_size };
                            minimap_tiles[idx + 3].position = { m_x * tile_size, (m_y + 1) * tile_size };

                            // Texture
                            const auto offset = get_texture_offset(type);
                            minimap_tiles[idx + 0].texCoords = { offset.x, offset.y };
                            minimap_tiles[idx + 1].texCoords = { offset.x + tex_width, offset.y };
                            minimap_tiles[idx + 2].texCoords = { offset.x + tex_width, offset.y + tex_height };
                            minimap_tiles[idx + 3].texCoords = { offset.x, offset.y + tex_height };

                            // Color
                            const float darkness = 150;
                            unsigned char color = type == 0 ? darkness : 255;
                            for(int i = 0; i < 4; ++i) setColor(minimap_tiles[idx + i], color);
                        }
                    }
                }

                // FOV
                const sf::Vector2f character_pos{posY * tile_size,posX * tile_size};
                const float character_dir_angle = -vecToAngle({dirX, dirY});
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
                    const unsigned char color = 255;
                    setColor(minimap_fov[0], color, 60);

                    // Then it goes invisible towards the end
                    for(int i = 1; i <= 2; ++i) setColor(minimap_fov[i], color, 0);
                }

                // Compass Arrow
                {
                    compass_arrow.setPosition(character_pos);
                    compass_arrow.setRotation(character_dir_angle);
                }

                // Minimap circle
                {
                    const sf::Vector2f minimap_rt_size(minimap_rt.getSize().x, minimap_rt.getSize().y);
                    const sf::Vector2f minimap_pos_offset = tile_size * -(0.5f * sf::Vector2f{mapWidth, mapHeight} + sf::Vector2f{-posY, -posX});
                    const sf::Vector2i minimap_rect_size(minimap_zoom * minimap_rt_size.x, minimap_zoom * minimap_rt_size.y);

                    // TODO: Character is not centered at borders. Instead of limiting the edges, we can have a bigger RenderTexture so minimap won't ever reach to the edges
                    minimap_circle.setTextureRect(sf::IntRect(
                            std::min(std::max(minimap_pos_offset.x + (1 - minimap_zoom) * 0.5f * minimap_rt_size.x, 0.0f), minimap_rt_size.x - minimap_rect_size.x),
                            std::min(std::max(minimap_pos_offset.y + (1 - minimap_zoom) * 0.5f * minimap_rt_size.y, 0.0f), minimap_rt_size.y - minimap_rect_size.y),
                            minimap_rect_size.x, minimap_rect_size.y));
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

                // Draw Minimap
                {
                    // Clear render texture
                    {
                        minimap_rt.clear(sf::Color::Black);
                    }

                    // Draw tiles to minimap render texture
                    {
                        minimap_rt.draw(minimap_tiles, &texture);
                    }

                    // Draw FOV to minimap render texture
                    {
                        minimap_rt.draw(minimap_fov);
                    }

                    // Draw compass character arrow
                    {
                        minimap_rt.draw(compass_arrow);
                    }

                    // Draw minimap circle
                    {
                        // Finalize minimap render texture
                        minimap_rt.display();

                        // Draw minimap circle
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

            // Print FPS
            {
                rt.draw(fps_text);
            }

            // Finalize render texture, draw it to the window, then finalize the window rendering
            {
                rt.display();
                window.draw(rt_sprite);
                window.display();
            }
        }
    }

    return EXIT_SUCCESS;
}
