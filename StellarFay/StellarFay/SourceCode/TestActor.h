#include "Actor.h"

class TestActor : public Actor
{
public:
	TestActor();

	~TestActor();

private:
	class ShaderWrapper * mShader;

	void UpdateActor() override;
};
