#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
class Functions {
public:
	glm::quat Euler_to_quat(double roll, double pitch, double yaw)
	{
		double cr = cos(roll * 0.5);
		double sr = sin(roll * 0.5);
		double cp = cos(pitch * 0.5);
		double sp = sin(pitch * 0.5);
		double cy = cos(yaw * 0.5);
		double sy = sin(yaw * 0.5);

		glm::quat q;
		q.w = cr * cp * cy + sr * sp * sy;
		q.x = sr * cp * cy - cr * sp * sy;
		q.y = cr * sp * cy + sr * cp * sy;
		q.z = cr * cp * sy - sr * sp * cy;

		return q;

	}
	glm::vec3 Quat_to_euler(const glm::quat& q)
	{
		double roll = atan2(2 * (q.w * q.x + q.y * q.z), 1 - 2 * (q.x * q.x + q.y * q.y));
		double pitch = asin(2 * (q.w * q.y - q.z * q.x));
		double yaw = atan2(2 * (q.w * q.z + q.x * q.y), 1 - 2 * (q.y * q.y + q.z * q.z));
		return glm::vec3(roll, pitch, yaw);
	}


	glm::vec3 Direction_to_forward(glm::vec3 vector3, double yaw, double pitch)
	{
		vector3.x = cos(yaw) * cos(pitch);
		vector3.y = sin(yaw) * cos(pitch);
		vector3.z = sin(pitch);

		return vector3;
	}


	glm::quat QuatLookAt(
		glm::vec3 const& from,
		glm::vec3 const& to,
		glm::vec3 const& up
	)
	{
		//you want to.y to be -
		glm::vec3  direction = to - from;
		float directionLength = glm::length(direction);

		if (!(directionLength > 0.0001))
			return glm::quat(1, 0, 0, 0);

		direction /= directionLength;

		if (glm::abs(glm::dot(direction, up)) > .9999f) {
			//you might to have add an alternative variable
			return glm::quatLookAt(direction, glm::vec3(0));
		}
		else {
			return glm::quatLookAt(direction, up);
		}
	}

	float Lerp(float from, float to, float time) {
		return from + time * (to - from);
	}
};