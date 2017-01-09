#ifndef CONVEYORONECONTROLLER_H_
#define CONVEYORONECONTROLLER_H_

/**
 * @file
 * @section DESCRIPTION
 *
 * Eine Klasse um den Motor des Foerderbandes an zu sprechen.
 */
class ConveyorOneController
{
public:
    ConveyorOneController();
    void shutdown();
    void action();
    virtual ~ConveyorOneController();
};

#endif /* CONVEYORONECONTROLLER_H_ */
