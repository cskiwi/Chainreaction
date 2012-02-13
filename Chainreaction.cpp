//-----------------------------------------------------------------
// http://www.digitalartsandentertainment.be/
//-----------------------------------------------------------------
#include "StdAfx.h"
#include "Resource.h"
//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Chainreaction.h"	

#include "Painter.h"
#include "ResourceLoader.h"
#include "InputState.h"
#include "CollisionMesh.h"
#include "Image.h"
#include "TextFormat.h"
#include "GameConfigurator.h"
#include "TextBox.h"
#include "Button.h"
#include "Audio.h"
#include "GameTimer.h"

#include "Ball.h"
#include "PlayField.h"
#include "MouseActions.h"

Chainreaction::Chainreaction():
	m_iActiveBalls(10),
	m_TextformatPtr(0),
	m_PlayFieldPtr(0),
	m_MouseActionsPtr(0),
	m_btnResetButtonPtr(0),
	m_txtAmountOfBallsPtr(0),
	m_iState(MENU),
	m_dTime(0),
	m_iGameResult(NOTPLAYED),
	m_iBallsNeededForLevel(m_iActiveBalls  * .9)
{
	for(int i=0; i<MAX_BALLS;i++)
		m_BallArrPtr[i] = 0;
}

Chainreaction::~Chainreaction()																						
{
	// nothing to destroy
}
void Chainreaction::GameInitialize(GameConfigurator* GameConfiguratorPtr)			
{
	GameConfiguratorPtr->SetWindowTitle("ChainReaction");
	GameConfiguratorPtr->SetWidth(700);
	GameConfiguratorPtr->SetHeight(700);

	GameConfiguratorPtr->EnableWindowResize(false);
	GameConfiguratorPtr->EnableWindowScale(true);

	GameConfiguratorPtr->SetNumPaintsPerSec(30);
	GameConfiguratorPtr->SetNumTicksPerSec(60);
}
void Chainreaction::GameStart(ResourceLoader* ResourceLoaderPtr)
{
	m_TextformatPtr = ResourceLoaderPtr->LoadTextFormat("arial",12);

	for(int i=0; i<MAX_BALLS;i++)
		m_BallArrPtr[i] = new Ball(ResourceLoaderPtr);

	m_PlayFieldPtr = new PlayField(500,500, ResourceLoaderPtr->GetWidth(), ResourceLoaderPtr->GetHeight());

	m_MouseActionsPtr = new MouseActions(ResourceLoaderPtr);

	m_btnResetButtonPtr = new Button(ResourceLoaderPtr->GetWindowHandle(),"Start");
	m_btnResetButtonPtr->AddActionListener(this);
	m_btnResetButtonPtr->SetBounds(10,10,75,25);

	String Text = "Max = ";
	Text += MAX_BALLS;

	m_txtAmountOfBallsPtr = new TextBox(ResourceLoaderPtr->GetWindowHandle(), Text);
	m_txtAmountOfBallsPtr->AddActionListener(this);
	m_txtAmountOfBallsPtr->SetBounds(10,40,75,20);
}
void Chainreaction::GameEnd()
{
	for(int i=0; i<MAX_BALLS;i++){
		delete m_BallArrPtr[i];
		m_BallArrPtr[i] = 0;
	}
	delete m_PlayFieldPtr;
	m_PlayFieldPtr = 0;

	delete m_TextformatPtr;
	m_TextformatPtr = 0;

	delete m_MouseActionsPtr;
	m_MouseActionsPtr = 0;

	delete m_btnResetButtonPtr;
	m_btnResetButtonPtr = 0;

	delete m_txtAmountOfBallsPtr;
	m_txtAmountOfBallsPtr = 0;
}
void Chainreaction::KeyUp(TCHAR CKey)
{
}
void Chainreaction::GameTick(InputState* InputStatePtr)
{

	switch(m_iState){
	case MENU:
		break;
	case GAME:
		// Tick Game elements
		for(int i=0; i<m_iActiveBalls;i++)
			m_BallArrPtr[i]->Tick(InputStatePtr,m_PlayFieldPtr);
		m_MouseActionsPtr->Tick(InputStatePtr,m_PlayFieldPtr);

		// collision tests
		int alreadyDone = 1;
		for (int i = 0; i < m_iActiveBalls;i++){
			for (int j = alreadyDone; j < m_iActiveBalls;j++){
				Hittest(m_BallArrPtr[i], m_BallArrPtr[j]);
			}
			alreadyDone++;
		}
		for (int i = 0; i < m_iActiveBalls;i++)
			Hittest(m_BallArrPtr[i],m_MouseActionsPtr);

		// Click check
		if (InputStatePtr->IsLeftMouseButtonClicked() && m_PlayFieldPtr->LocationInField(InputStatePtr->GetMousePos())){
			m_MouseActionsPtr->SetClicked();
			m_txtAmountOfBallsPtr->SetEnabled(false);
		}

		// Check if game needs to continue.
		static const int NORMAL = 0;
		static const int GONE = 4;

		bool GameEnded = false;
		bool ExplodedBallsLeft = false;
		int BallsGone = 0;
		for (int i = 0; i < m_iActiveBalls; i++){
			if (m_BallArrPtr[i]->GetState() != NORMAL && m_BallArrPtr[i]->GetState() != GONE) ExplodedBallsLeft = true;
			if (m_BallArrPtr[i]->GetState() == GONE) BallsGone++;
		}
		if(m_MouseActionsPtr->GetState() == GONE && ExplodedBallsLeft == false)
			GameEnded = true;

		if (GameEnded){
			if (m_dTime >= 1){
				m_iState = MENU;
				BallsGone >= m_iBallsNeededForLevel ? m_iGameResult = WON : m_iGameResult = LOST;
			} else 
					m_dTime += InputStatePtr->GetDeltaTime();
			
		}
		break;
	}
}
void Chainreaction::GamePaint(Painter* PainterPtr)
{
	switch(m_iState){
	case MENU:
		// paint playfield
		m_PlayFieldPtr->Paint(PainterPtr);

		// paint HUD
		PaintHUD(PainterPtr);
		break;
	case GAME:
		// paint playfield
		m_PlayFieldPtr->Paint(PainterPtr);

		// paint balls
		for(int i=0; i<m_iActiveBalls;i++)
			m_BallArrPtr[i]->Paint(PainterPtr, m_TextformatPtr);

		// paint HUD
		PaintHUD(PainterPtr);

		// paint mouseactions
		m_MouseActionsPtr->Paint(PainterPtr);
		break;
	}
}
void Chainreaction::CallAction(Caller* CallerPtr)
{
	if(CallerPtr == m_btnResetButtonPtr){
		Reset();
		if (m_btnResetButtonPtr->GetText() == "Start")
			m_btnResetButtonPtr->SetText("Reset");
	}
	if(CallerPtr == m_txtAmountOfBallsPtr){
		if (m_txtAmountOfBallsPtr->GetText().ToInteger() <= MAX_BALLS){
			m_iActiveBalls = m_txtAmountOfBallsPtr->GetText().ToInteger();
			m_iBallsNeededForLevel = m_iActiveBalls * .9;
		} else {
			String Text = "Max = ";
			Text += MAX_BALLS;
			Text += "!";
			m_txtAmountOfBallsPtr->SetText(Text);
		} 
	}
}
void Chainreaction::WindowResized(double Width, double Height)
{

}											// empty definition
void Chainreaction::PaintHUD(Painter* PainterPtr){
	int ExplodedBalls = 0;
	int Score = 0;
	for (int i = 0; i < m_iActiveBalls; i++)
		if (m_BallArrPtr[i]->GetState() != 0)
			ExplodedBalls ++;

	// set default collor
	PainterPtr->SetColor(255,255,255);
	// score n stuff
	String Text = "";
	Text += ExplodedBalls;
	Text += " balls exploded.";
	PainterPtr->DrawString(m_TextformatPtr,Text,Point2D(100,10));

	Text = "Balls needded for winning: ";
	Text += m_iBallsNeededForLevel;
	PainterPtr->DrawString(m_TextformatPtr,Text,Point2D(100,30));

	for (int i = 0; i < ExplodedBalls; i++)
		Score += 100*ExplodedBalls * 1.31;

	Text = "Score: ";
	Text += Score;
	PainterPtr->DrawString(m_TextformatPtr,Text,Point2D(100,50));

	// Textbox and button
	m_btnResetButtonPtr->Show();
	m_txtAmountOfBallsPtr->Show();

	// IF GameEnded
	if (m_iGameResult != NOTPLAYED)
		if (m_iGameResult == WON)
			PainterPtr->DrawString(m_TextformatPtr,"GAME OVER\nYou Won, nice", PainterPtr->GetWidth()/2 - 39, PainterPtr->GetHeight()/2 - 12) ;
		else if (m_iGameResult == LOST)
			PainterPtr->DrawString(m_TextformatPtr,"GAME OVER\nYou've LOST!", PainterPtr->GetWidth()/2 - 33, PainterPtr->GetHeight()/2  - 12);

}
void Chainreaction::Hittest(Ball* Ball1Ptr, Ball* Ball2Ptr){
	static const int NORMAL = 0;
	static const int GONE = 4;
	int ExplodedBalls = 0;
	for (int i = 0; i < m_iActiveBalls; i++)
		if (m_BallArrPtr[i]->GetState() != 0)
			ExplodedBalls ++;

	if (Ball1Ptr->GetState() != GONE && Ball2Ptr->GetState() != GONE){
		if(Ball1Ptr->GetState() != NORMAL) {
			if (Ball1Ptr->GetCollisionMesh()->HitTest(Ball2Ptr->GetCollisionMesh())){
				Ball2Ptr->Explode(100 * (ExplodedBalls * 1.31));
			}
		} else if(Ball2Ptr->GetState() != NORMAL) {
			if (Ball2Ptr->GetCollisionMesh()->HitTest(Ball1Ptr->GetCollisionMesh())){
				Ball1Ptr->Explode(100 * (ExplodedBalls * 1.31));
			}
		}
	}
}
void Chainreaction::Hittest(Ball* BallPtr, MouseActions* MouseActions){
	static const int NORMAL = 0;
	static const int GONE = 4;
	int ExplodedBalls = 0;
	for (int i = 0; i < m_iActiveBalls; i++)
		if (m_BallArrPtr[i]->GetState() != 0)
			ExplodedBalls ++;

	if (MouseActions->GetState() != GONE && BallPtr->GetState() != GONE)
		if(MouseActions->GetState() != NORMAL) 
			if (MouseActions->GetCollisionMesh()->HitTest(BallPtr->GetCollisionMesh()))
				BallPtr->Explode(100 * (ExplodedBalls * 1.31));


}
void Chainreaction::Reset(){
	for(int i=0; i<m_iActiveBalls;i++)
		m_BallArrPtr[i]->Reset();
	m_MouseActionsPtr->Reset();
	String Text = "Max = ";
	Text += MAX_BALLS;
	m_txtAmountOfBallsPtr->SetText(Text);
	m_txtAmountOfBallsPtr->SetEnabled(true);
	m_iState = GAME;
	m_iGameResult = NOTPLAYED;
}

