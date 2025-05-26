#include <functional>
#include "entity.hpp"

using Action = std::function<void ()>;

class Timer : public Entity {
private:
    Action action_;
    float duration_;
    float time_elapsed_ {};
    void update(float delta_time);

public:
    explicit Timer(Action action, float duration) :
        action_(std::move(action)),
        duration_(duration)
    {};

    void update() override;
};