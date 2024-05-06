#pragma  once

#include "../shaderProgram.hh"
#include "axis.hh"
#include "entity.hh"
#include "line.hh"
#include "points.hh"
#include <memory>

namespace entity {
class EntityFactory {
    public:
    EntityFactory();
    std::shared_ptr<Axis> create_axis(float ox, float oy, float oz, float length = 1.0f);
    std::shared_ptr<Points> create_points(const std::vector<GLPointData>& points);
    std::shared_ptr<Line> create_line(float p0_x, float p0_y, float p0_z, float p1_x, float p1_y, float p1_z, float r = 1.0, float g = 0.0,
       float b = 0.0);
    

    private:
    std::shared_ptr<ShaderProgram> point_program_;
    std::shared_ptr<ShaderProgram> line_program_;
    std::shared_ptr<ShaderProgram> axis_program_;
};
}