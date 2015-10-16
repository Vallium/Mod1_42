/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Drop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalliot <aalliot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/25 17:31:20 by aalliot           #+#    #+#             */
/*   Updated: 2015/09/25 17:31:23 by aalliot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Drop.hpp"
#include "Context.hpp"
#include "map.hpp"
#include <iostream>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/rotate_vector.hpp>

Drop::Drop(glm::vec3 pos) : _pos(pos) {
	_velocity = glm::vec3(0, 0, 0);
}

Drop::Drop(Drop const &cpy) : _pos(cpy.getPos()), _velocity(cpy.getVelocity()) {

}

glm::vec3	Drop::getPos(void) const {
	return (_pos);
}

glm::vec3	Drop::getVelocity(void) const{
	return (_velocity);
}

void		Drop::setPos(glm::vec3 pos) {
	_pos = pos;
}

void		Drop::setVelocity(glm::vec3 v) {
	_velocity = v;
}

void collision3D(float R, float m1, float m2, float r1, float r2,
                     float& x1, float& y1,float& z1,
                     float& x2, float& y2, float& z2,
                     float& vx1, float& vy1, float& vz1,
                     float& vx2, float& vy2, float& vz2,
                     int& error);

std::vector<Drop*>		Drop::update(Octree **dropsOctree, float dt) {
	std::vector<Drop*> drops;

	(*dropsOctree)->getPointsInsideBox(glm::vec3(0, 0, 0), glm::vec3(Context::size, Context::size, Context::size), drops);

	for (auto drop = drops.begin(); drop != drops.end(); ++drop) {
		glm::vec3 pos = (*drop)->getPos();
		glm::vec3 velocity = (*drop)->getVelocity();

		// Apply gravity
		velocity += glm::vec3(0.0f, (-9.81f * 100.0f) * dt, 0.0f);

		//Interpolate map Y
		float y1 = Context::map->interpolate(pos.x - 0.5f, pos.z - 0.5f);
		float y2 = Context::map->interpolate(pos.x + 0.5f, pos.z - 0.5f);
		float y3 = Context::map->interpolate(pos.x - 0.5f, pos.z + 0.5f);
		float y = (y1 + y2 + y3) / 3.00f;


		//Collision
		if (pos.y + velocity.y * dt <= y + DROP_PHYSIC_SIZE / 2.0f) {
			if (pos.y < y + DROP_PHYSIC_SIZE / 2.0f)
				pos.y = y + DROP_PHYSIC_SIZE / 2.0f;
			velocity.y = -velocity.y * 0.5f;
			velocity.x = -velocity.x * 0.5f;
			velocity.z = -velocity.z * 0.5f;

			glm::vec3	u(1, y2 - y1, 0);
			glm::vec3	v(0, y3 - y1, 1);
			glm::vec3	n = glm::normalize(glm::cross(v, u));
			velocity = glm::rotate(velocity, 180.0f, n);
		}

		if (pos.x + velocity.x * dt >= Context::size - DROP_PHYSIC_SIZE / 2.0f) {
			if (pos.x > Context::size - DROP_PHYSIC_SIZE / 2.0f)
				pos.x = Context::size - DROP_PHYSIC_SIZE / 2.0f;
			velocity.y = -velocity.y * 0.5f;
			velocity.x = -velocity.x * 0.5f;
			velocity.z = -velocity.z * 0.5f;

			glm::vec3	n = glm::vec3(-1, 0, 0);
			velocity = glm::rotate(velocity, 180.0f, n);
		}

		else if (pos.x + velocity.x * dt < 0 + DROP_PHYSIC_SIZE / 2.0f) {
			if (pos.x < DROP_PHYSIC_SIZE / 2.0f)
				pos.x = DROP_PHYSIC_SIZE / 2.0f;
			velocity.y = -velocity.y * 0.5f;
			velocity.x = -velocity.x * 0.5f;
			velocity.z = -velocity.z * 0.5f;

			glm::vec3	n = glm::vec3(1, 0, 0);
			velocity = glm::rotate(velocity, 180.0f, n);
		}

		if (pos.z + velocity.z * dt >= Context::size - DROP_PHYSIC_SIZE / 2.0f) {
			if (pos.z > Context::size - DROP_PHYSIC_SIZE / 2.0f)
				pos.z = Context::size - DROP_PHYSIC_SIZE / 2.0f;
			velocity.y = -velocity.y * 0.5f;
			velocity.x = -velocity.x * 0.5f;
			velocity.z = -velocity.z * 0.5f;

			glm::vec3	n = glm::vec3(0, 0, -1);
			velocity = glm::rotate(velocity, 180.0f, n);
		}
		else if (pos.z + velocity.z * dt < 0 + DROP_PHYSIC_SIZE / 2.0f) {
			if (pos.z < DROP_PHYSIC_SIZE / 2.0f)
				pos.z = DROP_PHYSIC_SIZE / 2.0f;
			velocity.y = -velocity.y * 0.5f;
			velocity.x = -velocity.x * 0.5f;
			velocity.z = -velocity.z * 0.5f;

			glm::vec3	n = glm::vec3(0, 0, 1);
			velocity = glm::rotate(velocity, 180.0f, n);
		}

		// Particles collide
		std::vector<Drop*> dropsNear;
		(*dropsOctree)->getPointsInsideBox(glm::vec3(pos.x - DROP_PHYSIC_SIZE, pos.y - DROP_PHYSIC_SIZE, pos.z - DROP_PHYSIC_SIZE), glm::vec3(pos.x + DROP_PHYSIC_SIZE, pos.y + DROP_PHYSIC_SIZE, pos.z + DROP_PHYSIC_SIZE), dropsNear);

		for (auto drop2 = dropsNear.begin(); drop2 != dropsNear.end(); ++drop2) {
			glm::vec3 pos2 = (*drop2)->getPos();
			if (*drop != *drop2) {
				glm::vec3       diffBefore(pos2.x - pos.x, pos2.y - pos.y, pos2.z - pos.z);
				glm::vec3       diffAfter(pos2.x - pos.x + velocity.x * dt, pos2.y - pos.y + velocity.y * dt, pos2.z - pos.z + velocity.z * dt);

				float	distBefore = sqrt(diffBefore.x * diffBefore.x + diffBefore.y * diffBefore.y + diffBefore.z * diffBefore.z);
				float	distAfter = sqrt(diffAfter.x * diffAfter.x + diffAfter.y * diffAfter.y + diffAfter.z * diffAfter.z);

				if (distAfter < DROP_PHYSIC_SIZE) {
					if (distBefore < DROP_PHYSIC_SIZE) {
						glm::vec3	move = glm::normalize(diffBefore);

						move.x *= -(DROP_PHYSIC_SIZE - distBefore);
						move.y *= -(DROP_PHYSIC_SIZE - distBefore);
						move.z *= -(DROP_PHYSIC_SIZE - distBefore);
						pos += move;
					}
					glm::vec3	pos2 = (*drop2)->getPos();
					glm::vec3	velocity2 = (*drop2)->getVelocity();
					int			error = 0;

					collision3D(1.0f, 1.0f, 1.0f, DROP_PHYSIC_SIZE / 2.0f, DROP_PHYSIC_SIZE / 2.0f, pos.x, pos.y, pos.z, pos2.x, pos2.y, pos2.z, velocity.x, velocity.y, velocity.z, velocity2.x, velocity2.y, velocity2.z, error);

					// if (error)
					// 	std::cout << error << std::endl;
					(*drop2)->setPos(pos2);
					(*drop2)->setVelocity(velocity2);
					break;
				}
			}
		}

		//Apply friction to velocity
		velocity.x = velocity.x - (velocity.x * 0.1f * dt); // Friction
		velocity.y = velocity.y - (velocity.y * 0.1f * dt); // Friction
		velocity.z = velocity.z - (velocity.z * 0.1f * dt); // Friction

		//Set new pos and velocity
		(*drop)->setPos(pos + velocity * dt);
		(*drop)->setVelocity(velocity);


	}
	return drops;
}

void collision3D(float R, float m1, float m2, float r1, float r2,
                     float& x1, float& y1,float& z1,
                     float& x2, float& y2, float& z2,
                     float& vx1, float& vy1, float& vz1,
                     float& vx2, float& vy2, float& vz2,
                     int& error)     {


       float  pi,r12,m21,d,v,theta2,phi2,st,ct,sp,cp,vx1r,vy1r,vz1r,fvz1r,
	           thetav,phiv,dr,alpha,beta,sbeta,cbeta,dc,sqs,t,a,dvz2,
			   vx2r,vy2r,vz2r,x21,y21,z21,vx21,vy21,vz21,vx_cm,vy_cm,vz_cm;

//     **** initialize some variables ****
       pi=acos(-1.0E0);
       error=0;
       r12=r1+r2;
       m21=m2/m1;
       x21=x2-x1;
       y21=y2-y1;
       z21=z2-z1;
       vx21=vx2-vx1;
       vy21=vy2-vy1;
       vz21=vz2-vz1;

       vx_cm = (m1*vx1+m2*vx2)/(m1+m2) ;
       vy_cm = (m1*vy1+m2*vy2)/(m1+m2) ;
       vz_cm = (m1*vz1+m2*vz2)/(m1+m2) ;


//     **** calculate relative distance and relative speed ***
       d=sqrt(x21*x21 +y21*y21 +z21*z21);
       v=sqrt(vx21*vx21 +vy21*vy21 +vz21*vz21);

//     **** return if distance between balls smaller than sum of radii ****
       if (d<r12) {error=2; return;}
//
//     **** return if relative speed = 0 ****
       if (v==0) {error=1; return;}

//     **** shift coordinate system so that ball 1 is at the origin ***
       x2=x21;
       y2=y21;
       z2=z21;

//     **** boost coordinate system so that ball 2 is resting ***
       vx1=-vx21;
       vy1=-vy21;
       vz1=-vz21;

//     **** find the polar coordinates of the location of ball 2 ***
       theta2=acos(z2/d);
       if (x2==0 && y2==0) {phi2=0;} else {phi2=atan2(y2,x2);}
       st=sin(theta2);
       ct=cos(theta2);
       sp=sin(phi2);
       cp=cos(phi2);


//     **** express the velocity vector of ball 1 in a rotated coordinate
//          system where ball 2 lies on the z-axis ******
       vx1r=ct*cp*vx1+ct*sp*vy1-st*vz1;
       vy1r=cp*vy1-sp*vx1;
       vz1r=st*cp*vx1+st*sp*vy1+ct*vz1;
       fvz1r = vz1r/v ;
       if (fvz1r>1) {fvz1r=1;}   // fix for possible rounding errors
          else if (fvz1r<-1) {fvz1r=-1;}
       thetav=acos(fvz1r);
       if (vx1r==0 && vy1r==0) {phiv=0;} else {phiv=atan2(vy1r,vx1r);}


//     **** calculate the normalized impact parameter ***
       dr=d*sin(thetav)/r12;


//     **** return old positions and velocities if balls do not collide ***
       if (thetav>pi/2 || fabs(dr)>1) {
           x2=x2+x1;
           y2=y2+y1;
           z2=z2+z1;
           vx1=vx1+vx2;
           vy1=vy1+vy2;
           vz1=vz1+vz2;
           error=1;
           return;
        }

//     **** calculate impact angles if balls do collide ***
       alpha=asin(-dr);
       beta=phiv;
       sbeta=sin(beta);
       cbeta=cos(beta);


//     **** calculate time to collision ***
       t=(d*cos(thetav) -r12*sqrt(1-dr*dr))/v;


//     **** update positions and reverse the coordinate shift ***
       x2=x2+vx2*t +x1;
       y2=y2+vy2*t +y1;
       z2=z2+vz2*t +z1;
       x1=(vx1+vx2)*t +x1;
       y1=(vy1+vy2)*t +y1;
       z1=(vz1+vz2)*t +z1;



//  ***  update velocities ***

       a=tan(thetav+alpha);

       dvz2=2*(vz1r+a*(cbeta*vx1r+sbeta*vy1r))/((1+a*a)*(1+m21));

       vz2r=dvz2;
       vx2r=a*cbeta*dvz2;
       vy2r=a*sbeta*dvz2;
       vz1r=vz1r-m21*vz2r;
       vx1r=vx1r-m21*vx2r;
       vy1r=vy1r-m21*vy2r;


//     **** rotate the velocity vectors back and add the initial velocity
//           vector of ball 2 to retrieve the original coordinate system ****

       vx1=ct*cp*vx1r-sp*vy1r+st*cp*vz1r +vx2;
       vy1=ct*sp*vx1r+cp*vy1r+st*sp*vz1r +vy2;
       vz1=ct*vz1r-st*vx1r               +vz2;
       vx2=ct*cp*vx2r-sp*vy2r+st*cp*vz2r +vx2;
       vy2=ct*sp*vx2r+cp*vy2r+st*sp*vz2r +vy2;
       vz2=ct*vz2r-st*vx2r               +vz2;


//     ***  velocity correction for inelastic collisions ***

       vx1=(vx1-vx_cm)*R + vx_cm;
       vy1=(vy1-vy_cm)*R + vy_cm;
       vz1=(vz1-vz_cm)*R + vz_cm;
       vx2=(vx2-vx_cm)*R + vx_cm;
       vy2=(vy2-vy_cm)*R + vy_cm;
       vz2=(vz2-vz_cm)*R + vz_cm;

       return;
}
