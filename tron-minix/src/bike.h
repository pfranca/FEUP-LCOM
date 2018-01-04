#ifndef __BIKE_H
#define __BIKE_H


/** @defgroup Bike
 * @{
 *
 * Bike Struct
 * Functions to manipulate the bike
 *
 */
typedef struct Bike{
	int initial_x_pos; 		/**<  initial X position  */
	int initial_y_pos; 		/**<  initial Y position  */
	int direction; 			/**<  direction  */
	int head_x_pos;			/**<  head X position  */
	int head_y_pos;			/**<  head Y position  */
	unsigned long color;	/**<  bike color  */
} Bike;

/**
 * @brief gets Initial X position
 *
 * @param bike the bike struct
 *
 * @return bike initial X position
 */
int getInitialX(struct Bike bike);

/**
 * @brief gets Initial Y position
 *
 * @param bike the bike struct
 *
 * @return bike initial Y position
 */
int getInitialY(struct Bike bike);

/**
 * @brief gets bike direction
 *
 * @param bike the bike struct
 *
 * @return bike direction
 */
int getDirection(struct Bike bike);

/**
 * @brief gets Head X position
 *
 * @param bike the bike struct
 *
 * @return bike  Head X position
 */
int getHeadX(struct Bike bike);

/**
 * @brief gets Head Y position
 *
 * @param bike the bike struct
 *
 * @return bike Head Y position
 */
int getHeadY(struct Bike bike);

/**
 * @brief gets the bike color
 *
 * @param bike the bike struct
 *
 * @return bike color
 */
unsigned long getcolor(struct Bike bike);

/**
 * @brief sets the bike direction
 *
 * @param bike the bike object
 *
 * @param dir the bike direction
 */
void setDirection(Bike *bike, int dir);


/**
 * @brief gets the bike color
 *
 * @param bike the bike object
 *
 * @param speed the bike  speed
 *
 */
void moveHead(Bike *bike, int speed);

#endif /* __BIKE_H */
