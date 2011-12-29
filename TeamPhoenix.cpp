#include "bzfsAPI.h"
#include "plugin_utils.h"

class TeamPhoenix : public bz_Plugin
{
	virtual const char* Name (){return "Team Phoenix";}
	virtual void Init ( const char* config);
	virtual void Event( bz_EventData *eventData );
	virtual void Cleanup ();
	
	float x[256], y[256], z[256], rot[256];
	bool newPosition[256];
};

BZ_PLUGIN(TeamPhoenix);

void TeamPhoenix::Init ( const char* /*commandLine*/ )
{
	bz_debugMessage(4,"TeamPhoenix plugin loaded");

    Register(bz_ePlayerDieEvent);
    Register(bz_eGetPlayerSpawnPosEvent);
    Register(bz_ePlayerPartEvent);
    Register(bz_ePlayerJoinEvent);
}

void TeamPhoenix::Cleanup(void)
{
	Flush();
}

void TeamPhoenix::Event(bz_EventData *eventData)
{
	switch (eventData->eventType)
	{
		case bz_ePlayerDieEvent:
		{
			bz_PlayerDieEventData_V1* dieData = (bz_PlayerDieEventData_V1*)eventData;
			
			if(dieData->killerID == 253)
			{
				newPosition[dieData->playerID] = true;
			}
			else
			{
				x[dieData->playerID] = dieData->state.pos[0];
				y[dieData->playerID] = dieData->state.pos[1];
				z[dieData->playerID] = dieData->state.pos[2];
				rot[dieData->playerID] = dieData->state.rotation;
			}
		}
		break;
		
		case bz_eGetPlayerSpawnPosEvent:
		{
			bz_GetPlayerSpawnPosEventData_V1* spawnData = (bz_GetPlayerSpawnPosEventData_V1*)eventData;
			
			if(newPosition[spawnData->playerID])
			{
				spawnData->handled=false;
				newPosition[spawnData->playerID] = false;
			}
			else
			{
				spawnData->handled = true;
				spawnData->pos[0] = x[spawnData->playerID];
				spawnData->pos[1] = y[spawnData->playerID];
				spawnData->pos[2] = z[spawnData->playerID];
				spawnData->rot = rot[spawnData->playerID];
			}
		}
		break;
		
		case bz_ePlayerPartEvent:
		{
			bz_PlayerJoinPartEventData_V1* partData = (bz_PlayerJoinPartEventData_V1*)eventData;
			
			x[partData->playerID] = '\0';
			y[partData->playerID] = '\0';
			z[partData->playerID] = '\0';
			rot[partData->playerID] = '\0';
		}
		break;
		
		case bz_ePlayerJoinEvent:
		{
			bz_PlayerJoinPartEventData_V1* joinData = (bz_PlayerJoinPartEventData_V1*)eventData;
			
			newPosition[joinData->playerID] = true;
		}
		break;
		
		default:break;
	}
}
