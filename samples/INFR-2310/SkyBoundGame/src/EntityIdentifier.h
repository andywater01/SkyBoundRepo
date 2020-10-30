#ifndef __ENTITYIDENTIFIER_H__
#define __ENTITYIDENTIFIER_H__

#include <string>

class EntityIdentifier
{
public:
	//Adds the bit identifier to the whole identifier
	void AddComponent(unsigned int component);

	//Getters
	//Gets main player entity
	static unsigned int MainPlayer();
	static unsigned int MainHeart();
	static unsigned int MainEnemy();
	bool GetIsMainPlayer() const;
	//Gets main camera entity
	static unsigned int MainCamera();
	bool GetIsMainCamera() const;
	//Get entity number
	unsigned int GetEntity() const;
	//Get identifier bitfield
	unsigned int GetIdentifier() const;
	//Get Name
	std::string GetName() const;
	//Get if this entity is selected
	bool& GetSelected();
	//Gets the actual bit for the specified type
	static unsigned int CameraBit();
	static unsigned int SpriteBit();
	static unsigned int TransformBit();
	static unsigned int AnimationBit();
	static unsigned int LinkBit();
	static unsigned int PhysicsBit();
	static unsigned int HealthBarBit();
	static unsigned int HoriScrollCameraBit();
	static unsigned int VertScrollCameraBit();
	static unsigned int PlayerBit();
	static unsigned int EnemyBit();
	static unsigned int AttackBit();
	static unsigned int AttackRightBit();

	//Setters
	//Sets main player entity
	static void MainPlayer(unsigned int entity);
	void SetIsMainPlayer(bool main);

	//Sets Hearts
	static void MainHeart(unsigned int entity);
	void SetIsMainHeart(bool main);

	static void MainEnemy(unsigned int entity);
	void SetIsMainEnemy(bool main);

	//Sets main camera entity
	static void MainCamera(unsigned int entity);
	void SetIsMainCamera(bool main);
	//Set entity number
	void SetEntity(unsigned int entity);
	//Set identifier bitfield
	void SetIdentifier(unsigned int identity);
	//Set the name
	void SetName(std::string name);
	//Set whether this entity is selected
	void SetSelected(bool selected);

private:
	//Main player
	static unsigned int m_mainPlayer;
	bool m_isMainPlayer = false;

	//Main Hearts
	static unsigned int m_mainHeart;
	bool m_isMainHeart = false;

	//Main enemy
	static unsigned int m_mainEnemy;
	bool m_isMainEnemy = false;

	//Main camera
	static unsigned int m_mainCamera;
	bool m_isMainCamera = false;
	static unsigned int m_slashEntity;
	static unsigned int m_heartEntity;



	//Stores the entity number
	unsigned int m_entity;
	//Stores the bit(s) of the types attached to this entity
	unsigned int m_identifier;

	//Stores the name
	std::string m_name = "";
	//Stores if this entity is selected
	bool m_selected = false;

	//Stores the bit for the specified type
	static unsigned int m_cameraBit;
	static unsigned int m_spriteBit;
	static unsigned int m_transformBit;
	static unsigned int m_animationBit;
	static unsigned int m_linkBit;
	static unsigned int m_physicsBit;
	static unsigned int m_healthBarBit;
	static unsigned int m_horiScrollCameraBit;
	static unsigned int m_vertScrollCameraBit;
	static unsigned int m_playerBit;
	static unsigned int m_enemyBit;
	static unsigned int m_attackBit;
	static unsigned int m_attackRightBit;
};

#endif // !__ENTITYIDENTIFIER_H__
