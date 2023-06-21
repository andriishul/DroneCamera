#pragma once
#include "glm/glm.hpp"
namespace cam {
	class Camera
	{
	private:
		glm::vec3 direction;
		glm::mat4 view;
		glm::vec3 this_pos, eye_pos, up_pos;
	public:

		Camera(glm::vec3&& position, glm::vec3&& up);
		//returns view matrix
		glm::mat4& operator*()
		{
			return view;
		}
		glm::vec3 position()const;
		
		void translate(glm::vec3 const& v);
		
		glm::vec3 forward()const;
		

		glm::vec3 right()const;
		
	private:
		void update();
		
	public:
		float yaw()const;
		
		float pitch()const;
		
		void rotate(float_t yaw, float_t pitch);
		
	};
}

