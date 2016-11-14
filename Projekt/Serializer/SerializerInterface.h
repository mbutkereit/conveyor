#ifndef SERIALIZERINTERFACE_H_
#define SERIALIZERINTERFACE_H_

#include <string>
using namespace std;

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um den Adapter(Ports) zugriff zu Managen.
 */
class SerializerInterface {
public:
	virtual ~SerializerInterface();
	virtual std::string serialize()=0;
	virtual std::string unserialize() = 0;
};
#endif /* SERIALIZERINTERFACE_H_ */
