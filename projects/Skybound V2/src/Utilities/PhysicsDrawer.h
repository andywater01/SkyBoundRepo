// Helper class; draws the world as seen by Bullet.
// This is very handy to see it Bullet's world matches yours
// How to use this class :
// Declare an instance of the class :
// 
// dynamicsWorld->setDebugDrawer(&mydebugdrawer);
// Each frame, call it :
// mydebugdrawer.SetMatrices(ViewMatrix, ProjectionMatrix);
// dynamicsWorld->debugDrawWorld();
#include "Utilities/BackendHandler.h"
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>


//GLuint VBO, VAO;
class PhysicsDrawer : public btIDebugDraw {
public:
	void SetMatrices(const Shader::sptr& shader, const glm::mat4& view, const glm::mat4& projection)
	{
		shader->Bind();
		shader->SetUniformMatrix("u_View", view);
		shader->SetUniformMatrix("u_Projection", projection);
	}
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		// Vertex data
		GLfloat points[12];

		points[0] = from.x();
		points[1] = from.y();
		points[2] = from.z();
		points[3] = color.x();
		points[4] = color.y();
		points[5] = color.z();

		points[6] = to.x();
		points[7] = to.y();
		points[8] = to.z();
		points[9] = color.x();
		points[10] = color.y();
		points[11] = color.z();

		if (!isInit)
		{
			//Gen the buffers and array
			glGenBuffers(1, &VBO);
			glGenVertexArrays(1, &VAO);

			//Initial setup
			//Bind everything
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			//Initial buffer allocation
			glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_DYNAMIC_DRAW);

			//Enabling the attrib arrays
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

			//Unbinding everything
			glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
			glBindVertexArray(GL_NONE);

			isInit = true;
		}
		else
		{
			//Bind everything
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			//Buffer subdata
			//DOES NOT REALLOCATE MEMORY (more efficient)
			//You are replacing the data all every frame BUT, it's WAY more efficient
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), &points);

			//Unbind everything
			glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
			glBindVertexArray(GL_NONE);
		}

		//Draw call
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(GL_NONE);

	}



	virtual void drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&) {}
	virtual void reportErrorWarning(const char*) {}
	virtual void draw3dText(const btVector3&, const char*) {}
	virtual void setDebugMode(int p) {
		m = p;
	}
	int getDebugMode(void) const { return 3; }
	int m;

private:
	GLuint VBO, VAO;
	bool isInit = false;
};