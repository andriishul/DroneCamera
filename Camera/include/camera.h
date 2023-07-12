#pragma once
#include "glm/glm.hpp"
namespace cam {
	
	//Base class of a camera with implementation for free flight capabilities
	class Camera
	{
	private:
		glm::mat4 view;
	protected:
		glm::vec3 center_pos, up_pos;
	public:
		Camera(glm::vec3&& position, glm::vec3&& up);
		//returns view matrix
		virtual glm::mat4& operator*()
		{
			return view;
		}
		virtual glm::vec3 position()const;
		
		virtual void translate(glm::vec3 const& v);
		
		virtual glm::vec3 forward()const;
		

		virtual glm::vec3 right()const;
		
	private:
		void update();
		
	public:
		virtual void set_position(glm::vec3 const& position);
		virtual float yaw()const;
		
		virtual float pitch()const;
		
		virtual void rotate(float_t yaw, float_t pitch);
		
	};
}

