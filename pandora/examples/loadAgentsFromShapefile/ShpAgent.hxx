
#ifndef __ShpAgent_hxx__
#define __ShpAgent_hxx__

#include <Agent.hxx>
#include <Action.hxx>

#include <string>

namespace Examples
{

class ShpAgent : public Engine::Agent
{
	std::string _label; // MpiStringAttribute
	int _intValue; // MpiBasicAttribute
	float _floatValue; // MpiBasicAttribute

public:
	// todo remove environment from here
	ShpAgent( const std::string & id );
	virtual ~ShpAgent();
	
	void updateState();
	void registerAttributes();
	void serialize();

	void setLabel( const std::string & label );
	void setIntValue( int intValue );
	void setFloatValue( float floatValue );

	const std::string getLabel();
	int getIntValue();
	float getFloatValue();


	////////////////////////////////////////////////
	// This code has been automatically generated //
	/////// Please do not modify it ////////////////
	////////////////////////////////////////////////
#ifdef PANDORAMPI
	ShpAgent( void * );
	void * fillPackage();
	void sendVectorAttributes(int);
	void receiveVectorAttributes(int);
#endif // PANDORAMPI
	////////////////////////////////////////////////
	//////// End of generated code /////////////////
	////////////////////////////////////////////////

};

} // namespace Examples

#endif // __ShpAgent_hxx__

