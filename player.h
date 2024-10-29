#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"
#include "elements.h"

#define MAX_WALK_VELOCITY 4
#define MAX_JUMP_VELOCITY 6
#define STEP_LENGTH 2

#define FACING_RIGHT 0
#define FACING_LEFT  7

#define PLAYER_STAND 0
#define PLAYER_MOVE 1
#define PLAYER_JUMPING 2
#define PLAYER_FALLING 3
#define PLAYER_DEAD 10

#define PLAYER_FACE_LEFT  0
#define PLAYER_FACE_RIGHT  4
#define PLAYER_JUMP_LEFT  2
#define PLAYER_JUMP_RIGHT  6
#define PLAYER_FALLING_LEFT  3
#define PLAYER_FALLING_RIGHT  7

struct Player
{
  unsigned char facing;
  unsigned char state;
  unsigned char step;
  unsigned char x;
  unsigned char y;  
  unsigned char x_velocity;
  unsigned char y_velocity;
  unsigned char count;
  unsigned char frame;
};

Player player;

void player_init() {
	player.x = 8;
	player.y = 0;
	player.x_velocity = 0;
	player.y_velocity = 0;
	player.facing = FACING_RIGHT;
	player.state = PLAYER_STAND;
	player.step = 0;
	player.count = 0;
  player.frame = 0;
}

void player_move (unsigned char new_facing) {
     if (player.state < PLAYER_FALLING) {
       //if we are still moving in the same direction, accelerate
       //player.state = PLAYER_MOVE;
       if (new_facing == player.facing)
       {
         if (player.x_velocity <= MAX_WALK_VELOCITY) player.x_velocity++;
       } else {
         player.facing = new_facing;
  
         //start with a little faster velocity when walking
         player.x_velocity = 2;
       }
     }
}

void player_jump() {
     //make sure that we are in a state where jumping is possible
     //(can't jump if already jumping/falling)
     if (player.state < PLAYER_JUMPING)
     {
        player.y_velocity = MAX_JUMP_VELOCITY;
        player.state = PLAYER_JUMPING;
     }    
}


void player_handle () {
      unsigned char tile_adjust;

      //handle delay for changing x velocity, y velocity, or when jumping (special case)
      if ((player.count >= MAX_WALK_VELOCITY - player.x_velocity) || (player.y_velocity > 0) || (player.state == PLAYER_JUMPING)) {
        player.frame++; 
        player.count = 0;
        
        //handle x axis
        if (player.x_velocity > 0) {
          if (player.facing == FACING_RIGHT) {
             //if we have hit someting moving right, adjust accordingly
		         if (backdrop_check_move_v(player.x + player.x_velocity + 8, player.y, tile_adjust) > 0){
                player.x = (tile_adjust * 8) - 9;
                player.x_velocity = 0;
             } else {
               //otherwise keep moving right
               if (player.x - backdrop.view_x > 54) {//37) {
                  backdrop_move_view(DIRECTION_RIGHT, player.x_velocity);
               } 
               player.x += player.x_velocity;
               player.x_velocity--;
             }
         } else {
          //facing left
          //if we have hit something moving left adjust accordingly
          if (backdrop_check_move_v(player.x - player.x_velocity, player.y, tile_adjust) > 0) {
            player.x = ((tile_adjust + 1) * 8) + 1;
            player.x_velocity = 0;    
          } else {
            //otherwise keep moving left
            if ((backdrop.view_x > 0) && (player.x < (backdrop.width_px - 56))) {
                  backdrop_move_view(DIRECTION_LEFT, player.x_velocity);    
            }             
            if (player.x - player.x_velocity > 0) player.x -= player.x_velocity;
            player.x_velocity--;
          }
         }
      }
      
      //handle y axis
      if (player.y_velocity > 0) {
        //moving up
        if (player.state == PLAYER_JUMPING){
           if (backdrop_check_move_h(player.x, player.y-player.y_velocity, tile_adjust) > 0) {
              player.y = ((tile_adjust) * 8);//((backdrop.view_adjust + 1) * 8)+ 1;
              player.y_velocity = 1;
              //starting to free fall
              player.state = PLAYER_FALLING;
           } else {
              if (player.y-player.y_velocity > 0) player.y -= player.y_velocity;
              //if ((backdrop.view_y > 0) && (player.y < (backdrop.height_px -56))) //** check this **/
              if ((backdrop.view_y > 0) && (player.y < (backdrop.height_px -64)))
              {
                  backdrop_move_view(DIRECTION_UP, player.y_velocity);    
              } 
              player.y_velocity--;
           }
        } else {
           if (backdrop_check_move_h(player.x, (player.y + player.y_velocity) + 8, tile_adjust) > 0) {
              player.y = (tile_adjust * 8) - 9;
              player.y_velocity = 0;
              //standing on ground
              player.state = PLAYER_STAND;
              
           } else {
              if ((player.y+player.y_velocity) > (backdrop.height_px - 9)) {
                player.state = PLAYER_DEAD;
              } else {
                player.y += player.y_velocity;
                if (player.y - backdrop.view_y > 23) {//43){
                   backdrop_move_view(DIRECTION_DOWN, player.y_velocity);    
                }                
                if (player.y_velocity <= MAX_JUMP_VELOCITY) player.y_velocity++;
              }
          }
        } 
      } else {
        //test for gravity pulling down (if nothing is below)
        if (backdrop_check_move_h(player.x, player.y+9, tile_adjust) == 0) {
            player.state = PLAYER_FALLING;
            player.y_velocity = 1;
        }
      }
                   
       //determine animation frames to show
	     if (player.y_velocity > 0) {
             if (player.state == PLAYER_FALLING) {
               if (player.facing == FACING_LEFT) player.frame = PLAYER_FALLING_LEFT;
               if (player.facing == FACING_RIGHT) player.frame = PLAYER_FALLING_RIGHT;
             } else {
               if (player.facing == FACING_LEFT) player.frame = PLAYER_JUMP_LEFT;
               if (player.facing == FACING_RIGHT) player.frame = PLAYER_JUMP_RIGHT;
             }
       } else {
          if (player.x_velocity > 0) {
             if ((player.facing == FACING_LEFT) && (player.frame > 1)) player.frame = PLAYER_FACE_LEFT;
             if ((player.facing == FACING_RIGHT) && (player.frame > 5)) player.frame = PLAYER_FACE_RIGHT;
          } else {
             if (player.facing == FACING_LEFT) player.frame = PLAYER_FACE_LEFT;
             if (player.facing == FACING_RIGHT) player.frame = PLAYER_FACE_RIGHT;
          }   
       }
    } else {
      player.count++;
    }
    //window.canvas.drawTile(window.playerTiles,player.frame, player.y - backdrop.view_y, player.x - backdrop.view_x);
    sprites.drawPlusMask(player.x - backdrop.view_x, player.y - backdrop.view_y, IMG_PLAYER, player.frame);
  }

/*
void player_handle () {
  unsigned char tile_adjust;
  
  if (player.state == PLAYER_SWINGING) {
    player.count++;
    if (player.count > 4) {
      player.step++; 
      if (player.step > 2) {
        player.step = 0;
        if (player.facing == FACING_LEFT) {
           player.facing = FACING_RIGHT;
        } else {
           player.facing = FACING_LEFT;
        }
      } else {
        if (player.facing == FACING_RIGHT) {
          //if (player.x<251) player.x += 3;
          if (player.x< backdrop.width_px - 5) player.x += 3;
          if (player.x - backdrop.view_x > 37) backdrop_move_view(DIRECTION_RIGHT, 3);
        } else {
          if (player.x > 3) player.x -= 3;
          //if ((backdrop.view_x > 0) && (player.x < 200)) backdrop_move_view(DIRECTION_LEFT, 3);
          if ((backdrop.view_x > 0) && (player.x < backdrop.width_px - 56)) backdrop_move_view(DIRECTION_LEFT, 3);
        }
      }
      player.count = 0;
    }
    
  } else { 
    if ((player.count >= MAX_WALK_VELOCITY - player.x_velocity) || (player.y_velocity > 0) || (player.state == PLAYER_JUMPING)){
  	player.step++; 
    if (player.step > 1) player.step = 0;
  	player.count = 0;
  	
  	//handle x axis
  	if (player.x_velocity > 0)
  	{
  		if (player.facing == FACING_RIGHT) {
  			//if we have hit something moving right, adjust accordingly
  			if (backdrop_check_move_v(player.x + player.x_velocity + 8, player.y, tile_adjust) > 0)
  			{
  				player.x = (tile_adjust * 8) - 9;
  				player.x_velocity = 0;
  			} else {
  				//if (player.x < 255)
          if (player.x < backdrop.width_px - 1) 
  				{
  					player.x += player.x_velocity;         
  					if (player.x - backdrop.view_x > 37)
  					{
  						backdrop_move_view(DIRECTION_RIGHT, player.x_velocity);    
  					} 
  					player.x_velocity--;
  				}
  			}
  		//facing left
  		} else {
  			if (backdrop_check_move_v(player.x - player.x_velocity, player.y, tile_adjust) > 0) {
  				player.x = ((tile_adjust + 1) * 8) + 1;
                  player.x_velocity = 0;			
  			} else {
  				
  				if (player.x - player.x_velocity > 0) player.x -= player.x_velocity;
  				//if ((backdrop.view_x > 0) && (player.x < 200))
          if ((backdrop.view_x > 0) && (player.x < backdrop.width_px - 56))
  				{
  					backdrop_move_view(DIRECTION_LEFT, player.x_velocity);    
  				} 
  				player.x_velocity--;
  			}
  		}	
  	}
  	
  	//handle y axis
  	if (player.y_velocity > 0) {
  		//moving up
  	    if (player.state == PLAYER_JUMPING)
  	    {
  		   if (backdrop_check_move_h(player.x, player.y-player.y_velocity, tile_adjust) > 0)
  		   {
  		      player.y = ((tile_adjust + 1) * 8) + 1;
  		      player.y_velocity = 1;
  	         //starting to free fall
  		     player.state = PLAYER_FALLING;
  		   } else {
            if (player.y-player.y_velocity > 0) player.y -= player.y_velocity;
            //if ((backdrop.view_y > 0) && (player.y < 216))
            if ((backdrop.view_y > 0) && (player.y < backdrop.height_px -56))
            {
                backdrop_move_view(DIRECTION_UP, player.y_velocity);    
            } 
  		      player.y_velocity--;
  		   }
  	    } else {
  		   if (backdrop_check_move_h(player.x, (player.y + player.y_velocity) + 8, tile_adjust) > 0)
  		   {
     		    player.y = (tile_adjust * 8) - 9;
  		      player.y_velocity = 0;
  		      //standing on ground
  		      player.state = PLAYER_STAND;
  		   } else {
           //if ((player.y+player.y_velocity) > 247) {
           if ((player.y+player.y_velocity) > backdrop.height_px - 9) {
  			     player.state = PLAYER_DEAD;
    			 } else {
    			      player.y += player.y_velocity;
                if (player.y - backdrop.view_y > 23)
                {
                  backdrop_move_view(DIRECTION_DOWN, player.y_velocity);    
                }                
    			     if (player.y_velocity <= MAX_JUMP_VELOCITY) player.y_velocity++;
            }
  		   }
  	   } 
  	} else {
  	    //test for gravity pulling down (if nothing is below)
  		if (backdrop_check_move_h(player.x, player.y+9, tile_adjust) == 0)
  		{
  		   player.state = PLAYER_FALLING;
  		   player.y_velocity = 1;
  		}
  	}
  	
    } else {
  	player.count++;
    }
  }
  //handle claw
  if (claw.state < CLAW_HIDDEN) {
    claw.step++;
    switch (claw.state) {
      case CLAW_RIGHT_OUT:
      if (claw.step > 0) sprites.drawSelfMasked((player.x - backdrop.view_x)+8, (player.y - backdrop.view_y)-8, IMG_CLAW, 0);
	    if (claw.step < 3) sprites.drawSelfMasked((player.x - backdrop.view_x)+16, (player.y - backdrop.view_y)-16, IMG_CLAW, 2);
      if (backdrop_check_claw(player.x+8, player.y))
      {
        //claw grabs a grabbable block
		//todo: if anything is in the path, do not continue
        player.x+=4;	
        player.y-=4;
        if (player.x - backdrop.view_x > 37) backdrop_move_view(DIRECTION_RIGHT, 4);
        //if ((backdrop.view_y > 0) && (player.y < 216)) backdrop_move_view(DIRECTION_UP, 2);
        if ((backdrop.view_y > 0) && (player.y < backdrop.height_px -56)) backdrop_move_view(DIRECTION_UP, 2);
        claw.state = CLAW_HIDDEN;
        player.state = PLAYER_SWINGING;
        player.step = 0;
      }
      if (claw.step > 3) {
        sprites.drawSelfMasked((player.x - backdrop.view_x)+16, (player.y - backdrop.view_y)-16, IMG_CLAW, 0);
		    sprites.drawSelfMasked((player.x - backdrop.view_x)+24, (player.y - backdrop.view_y)-24, IMG_CLAW, 2); 
        if (backdrop_check_claw(player.x + 16, player.y - 8))
        {
          //claw grabs a grabbable block
		  //todo: if anything is in the path, do not continue
          player.x+=8;
          player.y-=8;
          if (player.x - backdrop.view_x > 37) backdrop_move_view(DIRECTION_RIGHT, 8);
          //if ((backdrop.view_y > 0) && (player.y < 216)) backdrop_move_view(DIRECTION_UP, 6);
          if ((backdrop.view_y > 0) && (player.y < backdrop.height_px -56)) backdrop_move_view(DIRECTION_UP, 6);
          claw.state = CLAW_HIDDEN;
          player.state = PLAYER_SWINGING;
          player.step = 0;
        }
      }
      if (claw.step > 6) {
        claw.step = 0;
        claw.state = CLAW_RIGHT_IN;
      }
      break;

     case CLAW_RIGHT_IN:
      if (claw.step > 0) {
        sprites.drawSelfMasked((player.x - backdrop.view_x)+8, (player.y - backdrop.view_y)-8, IMG_CLAW, 0);
        sprites.drawSelfMasked((player.x - backdrop.view_x)+16, (player.y - backdrop.view_y)-16, IMG_CLAW, 2);
      }
      if (claw.step < 3) {
        sprites.drawSelfMasked((player.x - backdrop.view_x)+16, (player.y - backdrop.view_y)-16, IMG_CLAW, 0);
        sprites.drawSelfMasked((player.x - backdrop.view_x)+24, (player.y - backdrop.view_y)-24, IMG_CLAW, 2);   
      }
      if (claw.step > 6) {
        claw.step = 0;
        claw.state = CLAW_HIDDEN;
        player.state = PLAYER_STAND;
      }
      break;

      case CLAW_LEFT_OUT:
      if (claw.step > 0) sprites.drawSelfMasked((player.x - backdrop.view_x)-8, (player.y - backdrop.view_y)-8, IMG_CLAW, 1);
      if (claw.step < 3) sprites.drawSelfMasked((player.x - backdrop.view_x)-16, (player.y - backdrop.view_y)-16, IMG_CLAW, 3);
      if (backdrop_check_claw(player.x, player.y))
      {
       //claw grabs a grabbable block
		   //todo: if anything is in the path, do not continue
          player.x-= 4;
          player.y-=4;
		      //if ((backdrop.view_x > 0) && (player.x < 200)) backdrop_move_view(DIRECTION_LEFT, 4);
          if ((backdrop.view_x > 0) && (player.x < backdrop.width_px - 56)) backdrop_move_view(DIRECTION_LEFT, 4);
          //if ((backdrop.view_y > 0) && (player.y < 216)) backdrop_move_view(DIRECTION_UP, 2);
          if ((backdrop.view_y > 0) && (player.y < backdrop.height_px - 56)) backdrop_move_view(DIRECTION_UP, 2);
          claw.state = CLAW_HIDDEN;
          player.state = PLAYER_SWINGING;
          player.step = 0;
      }
      if (claw.step > 3) {
        sprites.drawSelfMasked((player.x - backdrop.view_x)-16, (player.y - backdrop.view_y)-16, IMG_CLAW, 1);
        sprites.drawSelfMasked((player.x - backdrop.view_x)-24, (player.y - backdrop.view_y)-24, IMG_CLAW, 3); 
        if (backdrop_check_claw(player.x  - 8, player.y - 8))
        {
          //claw grabs a grabbable block
		  //todo: if anything is in the path, do not continue
          player.x-= 8;
          player.y-=8;
          //if ((backdrop.view_x > 0) && (player.x < 200)) backdrop_move_view(DIRECTION_LEFT, 8);
          if ((backdrop.view_x > 0) && (player.x < backdrop.width_px - 56)) backdrop_move_view(DIRECTION_LEFT, 8);
          //if ((backdrop.view_y > 0) && (player.y < 216)) backdrop_move_view(DIRECTION_UP, 6);
          if ((backdrop.view_y > 0) && (player.y < backdrop.height_px - 56)) backdrop_move_view(DIRECTION_UP, 6);	  
          claw.state = CLAW_HIDDEN;
          player.state = PLAYER_SWINGING;
          player.step = 0;
        }
      }
      if (claw.step > 6) {
        claw.step = 0;
        claw.state = CLAW_LEFT_IN;
      }
      break;

       case CLAW_LEFT_IN:
       if (claw.step > 0) {
        sprites.drawSelfMasked((player.x - backdrop.view_x)-8, (player.y - backdrop.view_y)-8, IMG_CLAW, 1);
        sprites.drawSelfMasked((player.x - backdrop.view_x)-16, (player.y - backdrop.view_y)-16, IMG_CLAW, 3);
      }
      if (claw.step < 3) {
        sprites.drawSelfMasked((player.x - backdrop.view_x)-16, (player.y - backdrop.view_y)-16, IMG_CLAW, 1);
        sprites.drawSelfMasked((player.x - backdrop.view_x)-24, (player.y - backdrop.view_y)-24, IMG_CLAW, 3);   
      }
      if (claw.step > 6) {
        claw.step = 0;
        claw.state = CLAW_HIDDEN;
        player.state = PLAYER_STAND;
      }
      break;
    }

  }

  if ((player.x_velocity > 0 && player.state == PLAYER_MOVE) || player.state == PLAYER_SWINGING) {
    sprites.drawOverwrite(player.x - backdrop.view_x, player.y - backdrop.view_y, IMG_PLAYER, player.state + player.facing + player.step);
  } else {
    sprites.drawOverwrite(player.x - backdrop.view_x, player.y - backdrop.view_y, IMG_PLAYER, player.state + player.facing);
  }
}
*/

#endif
