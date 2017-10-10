#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum Camera_Movement{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 2.5f;
const float SENSITIVITY = 0.02f;
const float ZOOM        = 45.0f;

class Camera
{
public:
	glm::vec3 mPosition;
	glm::vec3 mFront;
	glm::vec3 mUp;
	glm::vec3 mRight;
	glm::vec3 mWorldUp;
	
	float mYaw;
	float mPitch;
	
	float mMovementSpeed;
	float mMouseSensitivity;
	float mZoom;
	
public:
	Camera(glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f),glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f),float yaw = YAW,float pitch = PITCH)
		: mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mMovementSpeed(SPEED), mMouseSensitivity(SENSITIVITY), mZoom(ZOOM)
	{
		mPosition = position;
        mWorldUp = up;
        mYaw = yaw;
        mPitch = pitch;
        updateCameraVectors();
	}
	
	Camera(float posX,float posY,float posZ,float upX,float upY,float upZ,float yaw,float pitch)
		: mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mMovementSpeed(SPEED), mMouseSensitivity(SENSITIVITY), mZoom(ZOOM)
	{
		mPosition = glm::vec3(posX,posY,posZ);
		mWorldUp = glm::vec3(upX,upY,upZ);
		mYaw = yaw;
		mPitch = pitch;
		updateCameraVectors();
	}
	
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(mPosition,mPosition + mFront,mUp);
	}
	
	void ProcessKeyboard(Camera_Movement direction,float deltaTime)
	{
		float velocity = mMovementSpeed * deltaTime;
		
		if(direction == FORWARD)
		{
			mPosition += mFront * velocity;
		}
		
		if(direction == BACKWARD)
		{
			mPosition -= mFront * velocity;
		}
		
		if(direction == LEFT)
		{
			mPosition -= mRight * velocity;
		}
		
		if(direction == RIGHT)
		{
			mPosition += mRight * velocity;
		}
	}
	
	void ProcessMouseMovement(float xoffset,float yoffset,GLboolean constrainPitch = true)
	{
		xoffset *= mMouseSensitivity;
		yoffset *= mMouseSensitivity;
		
		mYaw += xoffset;
		mPitch += yoffset;
		
		if(constrainPitch)
		{
			if(mPitch > 89.0f)
				mPitch = 89.0f;
			if(mPitch < -89.0f)
				mPitch = -89.0f;
		}
		
		updateCameraVectors();
	}
	
	void ProcessMouseScroll(float yoffset)
    {
        if (mZoom >= 1.0f && mZoom <= 60.0f)
            mZoom -= yoffset;
        if (mZoom <= 1.0f)
            mZoom = 1.0f;
        if (mZoom >= 60.0f)
            mZoom = 60.0f;
    }
	
private:
	
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		front.y = sin(glm::radians(mPitch));
		front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		
		mFront = glm::normalize(front);
		mRight = glm::normalize(glm::cross(mFront,mWorldUp));
		mUp = glm::normalize(glm::cross(mRight,mFront));
	}
};
