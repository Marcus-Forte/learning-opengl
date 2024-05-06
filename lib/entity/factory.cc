#include "entity/factory.hh"
#include "shaderProgram.hh"
#include <memory>

namespace entity {
    EntityFactory::EntityFactory() {
        axis_program_ = std::make_shared<ShaderProgram>("../shaders/line-vert.glsl", "../shaders/fragment.glsl");
        line_program_ = std::make_shared<ShaderProgram>("../shaders/line-vert.glsl", "../shaders/fragment.glsl");
        point_program_ = std::make_shared<ShaderProgram>("../shaders/point-vert.glsl", "../shaders/point-frag.glsl");
    }
    std::shared_ptr<Axis> EntityFactory::create_axis(float ox, float oy, float oz, float length) {
        return std::make_shared<Axis>(axis_program_, ox, oy, oz, length);
    }
    std::shared_ptr<Points> EntityFactory::create_points(const std::vector<GLPointData>& points) {
        return std::make_shared<Points>(point_program_, points);
    }
    std::shared_ptr<Line> EntityFactory::create_line(float p0_x, float p0_y, float p0_z, float p1_x, float p1_y, float p1_z, float r, float g, float b) {
        return std::make_shared<Line>(line_program_, p0_x, p0_y, p0_z, p1_x, p1_y, p1_z, r,g,b);
    }
}