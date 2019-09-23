#include "AppClass.h"
void Application::InitVariables(void)
{
	/* Old Code
	//Make MyMesh object
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(2.0f, C_BROWN);

	//Make MyMesh object
	m_pMesh1 = new MyMesh();
	m_pMesh1->GenerateCube(1.0f, C_WHITE);
	*/
	//My code
	offSet = 0.0f;
	movingRight = true;
	for (int i = 0; i < 46; i++)
	{
		MyMesh* newBlock = new MyMesh();
		newBlock->GenerateCube(1.0f, C_BLACK);
		collection.push_back(newBlock);
	}

}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();
	/*
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), ToMatrix4(m_qArcBall));
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(),));
	*/
	if (movingRight && offSet < 2.0f)
	{
		offSet += 0.1f;
	}
	else if(movingRight)
	{
		movingRight = false;
	}
	if (!movingRight && offSet > -2.0f)
	{
		offSet -= 0.1f;
	}
	else if(!movingRight)
	{
		movingRight = true;
	}

	for ( int i = 0; i < 46; i++)
	{
#pragma region Unholy code i'm sorry
		if (i <=10)
		{
			collection[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(i + offSet-5,2.0f, 0.0f)));
		}
		else if (i > 10 && i <=17)
		{
			collection[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(i + offSet - 14, 4.0f, 0.0f)));
		}
		else if (i > 17 && i <= 24)
		{
			collection[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(i + offSet - 21, 1.0f, 0.0f)));
		}
		else if (i > 24 && i <= 31)
		{
			if (i < 27)
			{
				collection[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(i + offSet - 29, 3.0f, 0.0f)));
			}
			else if (i > 29)
			{
				collection[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(i + offSet - 27, 3.0f, 0.0f)));
			}
			else
			{
				collection[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(i + offSet - 28, 3.0f, 0.0f)));
			}
		}
		else
		{
			if (i < 34)
			{
				collection[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(i + offSet - 34, -1.0f, 0.0f)));
			}
			else if(i < 36)
			{
				collection[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(i + offSet - 33, -1.0f, 0.0f)));
			}
			else if (i < 38)
			{
				if (i %2 == 0)
				{
					collection[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(i + offSet - 41, 1.0f, 0.0f)));
				}
				else
				{
					collection[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(i + offSet - 42, 0.0f, 0.0f)));

				}
			}
			else if (i < 40)
			{
				if (i % 2 == 0)
				{
					collection[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(i + offSet - 33, 1.0f, 0.0f)));
				}
				else
				{
					collection[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(i + offSet - 34, 0.0f, 0.0f)));

				}
			}
			else if(i < 42)
			{
				if (i % 2 == 0)
				{
					collection[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(i + offSet - 43, 0.0f, 0.0f)));
				}
				else
				{
					collection[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(i + offSet - 38, 0.0f, 0.0f)));

				}
			}
			else
			{
				if ( i == 42)
				{
					collection[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(i + offSet - 44, 5.0f, 0.0f)));
				}
				else if (i == 43)
				{
					collection[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(i + offSet - 41, 5.0f, 0.0f)));
				}
				else if (i == 44)
				{
					collection[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(i + offSet - 47, 6.0f, 0.0f)));
				}
				else
				{
					collection[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(i + offSet - 42, 6.0f, 0.0f)));
				}
			}
		}
#pragma endregion
	}


	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	for (int i = 0; i < 46; i++)
	{
		if (collection[i] != nullptr)
		{
			delete collection[i];
			collection[i] = nullptr;
		}
		SafeDelete(collection[i]);

	}

	//release GUI
	ShutdownGUI();
}