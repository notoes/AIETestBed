#include "OpenGLRenderer.h"

#include "Mesh.h"

using namespace reng;

void OpenGLRenderer::UseProgram( ProgramId programId )
{
	if (m_state.programId != programId) {
		glUseProgram(programId.Value());
		m_state.programId = programId;
	}
}

void OpenGLRenderer::Bind( const Mesh& mesh )
{
	if (m_state.vaoId != mesh.vaoId) {
		glBindVertexArray(mesh.vaoId.Value());
		m_state.vaoId = mesh.vaoId;
	}
}

void OpenGLRenderer::Draw(const Mesh& mesh )
{
	if (mesh.iboId == IBOId_Invalid) {
		glDrawArrays(GL_TRIANGLES, 0, mesh.numberOfVerts);
	}
	else {
		glDrawElements(GL_TRIANGLES, mesh.numberOfIndices, mesh.indexType, 0);
	}
}

void OpenGLRenderer::Unbind()
{
	glBindVertexArray(0);
	m_state.vaoId = VAOId_Invalid;
}


void OpenGLRenderer::UnuseProgram(){
	glUseProgram(0);
	m_state.programId = ProgramId_Invalid;
}
