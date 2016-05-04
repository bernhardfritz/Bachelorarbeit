#ifndef FOOTEST_H
#define FOOTEST_H

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

#include <vector>
#include <algorithm>
#include <math.h>

class MyRayCastCallback : public b2RayCastCallback
{
  public:
  int m_count;
  std::vector<b2Vec2> m_points;
  std::vector<b2Vec2> m_normals;
  std::vector<b2Body*> m_bodies;
  std::vector<b2PolygonShape*> m_polygonShapes;

	MyRayCastCallback() : m_count(0) {}

	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
	{
		b2Body* body = fixture->GetBody();
    b2PolygonShape* polygonShape = (b2PolygonShape*)fixture->GetShape();
		void* userData = body->GetUserData();
		if (userData)
		{
			int32 index = *(int32*)userData;
			if (index == 0)
			{
				// By returning -1, we instruct the calling code to ignore this fixture
				// and continue the ray-cast to the next fixture.
				return -1.0f;
			}
		}

		m_points.push_back(point);
		m_normals.push_back(normal);
    m_bodies.push_back(body);
    m_polygonShapes.push_back(polygonShape);
    m_count++;

		// By returning 1, we instruct the caller to continue without clipping the ray.
		return 1.0f;
	}
};

class MyContactListener : public b2ContactListener
{
  void BeginContact(b2Contact* contact) {
    if(contact->GetManifold()->pointCount < 2) return;

    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);
    b2Vec2 point1 = worldManifold.points[0];
    b2Vec2 point2 = worldManifold.points[1];

    bool collisionWithStaticBody = false;
    void* userData = contact->GetFixtureA()->GetBody()->GetUserData();
    if ( userData ) {
      int32 index = *(int32*)userData;
      if(index == 0) collisionWithStaticBody = true;
    } else {
    }

    userData = contact->GetFixtureB()->GetBody()->GetUserData();
    if ( userData ) {
      int32 index = *(int32*)userData;
      if(index == 0) collisionWithStaticBody = true;
    } else {
    }

    if(!collisionWithStaticBody) printf("Collison at: %.2f %.2f, %.2f %.2f\n", point1.x, point1.y, point2.x, point2.y);
  }

  void EndContact(b2Contact* contact) {
    void* userData = contact->GetFixtureA()->GetBody()->GetUserData();
    if ( userData )
      int32 index = *(int32*)userData;
    else {
    }
    userData = contact->GetFixtureB()->GetBody()->GetUserData();
    if ( userData )
      int32 index = *(int32*)userData;
    else {
    }
  }
};


class FooTest : public Test
{
  public:
  b2Body* kinematicBodies[4];
  b2Vec2 p1;
  b2Vec2 p2;
  bool pointsAreInitialized;
  bool mouseIsDown;
  int32 bodyIgnore;
  int32 bodyConsider;

	FooTest() : pointsAreInitialized(false), mouseIsDown(false), bodyIgnore(0), bodyConsider(1) {
    //body definition
    b2BodyDef myBodyDef;

    //shape definition
    b2PolygonShape polygonShape;

    //fixture definition
    b2FixtureDef myFixtureDef;
    myFixtureDef.shape = &polygonShape;
    myFixtureDef.density = 1;
    myFixtureDef.restitution = 0.5;

    //kinematic bodies
    myBodyDef.type = b2_kinematicBody;
    myBodyDef.position.Set(0, 0);
    myBodyDef.userData = &bodyIgnore;
    for(int i = 0; i < 4; i++) {
        kinematicBodies[i] = m_world->CreateBody(&myBodyDef);
    }

    //add four walls to the static body
    polygonShape.SetAsBox( 20, 1, b2Vec2(0, 0.5), 0);//ground
    kinematicBodies[0]->CreateFixture(&myFixtureDef);
    kinematicBodies[0]->SetLinearVelocity(b2Vec2(0,1));
    polygonShape.SetAsBox( 20, 1, b2Vec2(0, 40.5), 0);//ceiling
    kinematicBodies[1]->CreateFixture(&myFixtureDef);
    kinematicBodies[1]->SetLinearVelocity(b2Vec2(0,1));
    polygonShape.SetAsBox( 1, 20, b2Vec2(-20.5, 20), 0);//left wall
    kinematicBodies[2]->CreateFixture(&myFixtureDef);
    kinematicBodies[2]->SetLinearVelocity(b2Vec2(1,0));
    polygonShape.SetAsBox( 1, 20, b2Vec2(19.5, 20), 0);//right wall
    kinematicBodies[3]->CreateFixture(&myFixtureDef);
    kinematicBodies[3]->SetLinearVelocity(b2Vec2(1,0));

    myBodyDef.type = b2_dynamicBody;
    myBodyDef.position.Set(0,20);
    myBodyDef.userData = &bodyConsider;
    polygonShape.SetAsBox(18,18);
    m_world->CreateBody(&myBodyDef)->CreateFixture(&myFixtureDef);

    m_world->SetContactListener(new MyContactListener());

    m_world->SetGravity(b2Vec2(0, 0));

    srand48(time(NULL));
    randomCut(10);
  }

  //override parent class
  void MouseDown(const b2Vec2& p)
  {
    mouseIsDown = true;

    //store last mouse-down position
    p1 = p;

    //do normal behaviour
    Test::MouseDown(p);
  }

  void MouseUp(const b2Vec2& p)
  {
    mouseIsDown = false;
    p2 = p;
    pointsAreInitialized = true;
    Test::MouseUp(p);
  }

  void Cut(b2Vec2 from, b2Vec2 to)
  {
    bool pointsAreEqual = from.x == to.x && from.y == to.y;
    if(pointsAreEqual) return;
    std::vector<b2Body*> affectedBodies;
    std::vector<b2PolygonShape*> affectedPolygonShapes;
    std::vector<b2Vec2> points;
    MyRayCastCallback callback1;
    m_world->RayCast(&callback1, from, to);
    for(int i = 0; i < callback1.m_count; i++) {
      points.push_back(callback1.m_points[i]);
      m_debugDraw.DrawPoint(points.back(), 5.0f, b2Color(0.9f, 0.4f, 0.4f));
      if(std::find(affectedBodies.begin(), affectedBodies.end(), callback1.m_bodies[i]) == affectedBodies.end()) { // does not contain
        affectedBodies.push_back(callback1.m_bodies[i]);
        affectedPolygonShapes.push_back(callback1.m_polygonShapes[i]);
      }
    }
    MyRayCastCallback callback2;
    m_world->RayCast(&callback2, to, from);
    for(int i = 0; i < callback2.m_count; i++) {
      int index = find(affectedBodies.begin(), affectedBodies.end(), callback2.m_bodies[i]) - affectedBodies.begin();
      if(index < affectedBodies.size()) { // does contain
        points.push_back(callback2.m_points[i]);
        m_debugDraw.DrawPoint(points.back(), 5.0f, b2Color(0.4f, 0.4f, 0.9f));
        b2Vec2 rayCenter((points.back().x + points[index].x) / 2.0f, (points.back().y + points[index].y) / 2.0f);
        m_debugDraw.DrawPoint(rayCenter, 5.0f, b2Color(0.4f, 0.9f, 0.4f));
        float rayAngle = atan2f(points[index].y - points.back().y, points[index].x - points.back().x);
        std::vector<b2Vec2> vertices1;
        std::vector<b2Vec2> vertices2;
        for(int j = 0; j < affectedPolygonShapes[index]->GetVertexCount(); j++) {
          b2Vec2 polygonVertex = affectedBodies[index]->GetWorldPoint(affectedPolygonShapes[index]->GetVertex(j));
          float cutAngle = atan2f(polygonVertex.y - rayCenter.y, polygonVertex.x - rayCenter.x) - rayAngle;
          if(cutAngle < M_PI*-1.0f) {
            cutAngle += 2.0f * M_PI;
          }
          if(cutAngle > 0.0f && cutAngle <= M_PI) {
            vertices1.push_back(polygonVertex);
            m_debugDraw.DrawPoint(polygonVertex, 5.0f, b2Color(0.4f, 0.9f, 0.9f));
          } else {
            vertices2.push_back(polygonVertex);
            m_debugDraw.DrawPoint(polygonVertex, 5.0f, b2Color(0.9f, 0.9f, 0.4f));
          }
        }
        vertices1.push_back(points[index]);
        vertices1.push_back(points.back());

        vertices2.push_back(points.back());
        vertices2.push_back(points[index]);

        if(!mouseIsDown) {
          bool cutLegitimate = true;
          for(int j = 0; j < vertices1.size() && cutLegitimate; j++) {
            b2Vec2 edge = vertices1[(j + 1) % vertices1.size()] - vertices1[j];
            if(edge.Length() <= 0.1f) cutLegitimate = false;
          }

          for(int j = 0; j < vertices2.size() && cutLegitimate; j++) {
            b2Vec2 edge = vertices2[(j + 1) % vertices2.size()] - vertices2[j];
            if(edge.Length() <= 0.1f) cutLegitimate = false;
          }

          bool polygonDegenerate = false;
          float epsilon = 0.1f;
          for(int j = 0; j < vertices1.size() && !polygonDegenerate; j++) {
            b2Vec2 v1 = vertices1[j];
            for(int k = 0; k < vertices1.size() && !polygonDegenerate; k++) {
              if(k == j) continue;
              b2Vec2 v2 = vertices1[k];
              if(fabs(v1.x - v2.x) < epsilon && fabs(v1.y - v2.y) < epsilon) polygonDegenerate = true;
            }
          }

          for(int j = 0; j < vertices2.size() && !polygonDegenerate; j++) {
            b2Vec2 v1 = vertices2[j];
            for(int k = 0; k < vertices2.size() && !polygonDegenerate; k++) {
              if(k == j) continue;
              b2Vec2 v2 = vertices2[k];
              if(fabs(v1.x - v2.x) < epsilon && fabs(v1.y - v2.y) < epsilon) polygonDegenerate = true;
            }
          }

          if(vertices1.size() < 3 || vertices1.size() > 8 || vertices2.size() < 3 || vertices2.size() > 8 || polygonDegenerate) {
            if(i < callback2.m_count - 1) to = callback2.m_points[i+1];
            else to = from;
            continue;
          }

          b2Vec2 center1(0,0);
          b2Vec2 center2(0,0);
          for(int i = 0; i < vertices1.size(); i++) {
            center1 += vertices1[i];
          }
          center1 *= 1.0f / vertices1.size();
          for(int i = 0; i < vertices1.size(); i++) {
            vertices1[i] -= center1;
          }

          for(int i = 0; i < vertices2.size(); i++) {
            center2 += vertices2[i];
          }
          center2 *= 1.0f / vertices2.size();
          for(int i = 0; i < vertices2.size(); i++) {
            vertices2[i] -= center2;
          }

          //body definition
          b2BodyDef myBodyDef;
          myBodyDef.type = b2_dynamicBody;

          //shape definition
          b2PolygonShape polygonShape;

          //fixture definition
          b2FixtureDef myFixtureDef;
          myFixtureDef.shape = &polygonShape;
          myFixtureDef.density = 1;
          myFixtureDef.restitution = 0.5;

          myBodyDef.position.Set(center1.x, center1.y);
          polygonShape.Set(&vertices1[0], vertices1.size());
          b2Body* body1 = m_world->CreateBody(&myBodyDef);
          body1->CreateFixture(&myFixtureDef);
          body1->SetLinearVelocity(affectedBodies[index]->GetLinearVelocity());
          body1->SetAngularVelocity(affectedBodies[index]->GetAngularVelocity());

          myBodyDef.position.Set(center2.x, center2.y);
          polygonShape.Set(&vertices2[0], vertices2.size());
          b2Body* body2 = m_world->CreateBody(&myBodyDef);
          body2->CreateFixture(&myFixtureDef);
          body2->SetLinearVelocity(affectedBodies[index]->GetLinearVelocity());
          body2->SetAngularVelocity(affectedBodies[index]->GetAngularVelocity());

          if(i < callback2.m_count - 1) to = callback2.m_points[i+1];
          else to = from;
          m_world->DestroyBody(affectedBodies[index]);
        }
      }
    }
    m_debugDraw.DrawSegment(from, to, b2Color(0.8f, 0.8f, 0.8f));
  }

  void randomCut(int n = 1) {
    for(int i = 0; i < n; i++) {
      if(drand48() < 0.5) {
        b2Vec2 v1(-20, drand48() * 40);
        b2Vec2 v2(20, drand48() * 40);
        Cut(v1, v2);
      } else {
        b2Vec2 v1(drand48() * 40 - 20, 0);
        b2Vec2 v2(drand48() * 40 - 20, 40);
        Cut(v1, v2);
      }
    }
  }

  void Step(Settings* settings)
  {
    //run the default physics and rendering
    Test::Step(settings);

    m_debugDraw.DrawString(5, m_textLine, "p1:%.3f,%.3f", p1.x, p1.y);
    m_textLine += 15;
    m_debugDraw.DrawString(5, m_textLine, "p2:%.3f,%.3f", p2.x, p2.y);

    if(mouseIsDown) {
      p2 = m_mouseWorld;
      pointsAreInitialized = true;
    }

    if(pointsAreInitialized) {
      Cut(p1, p2);
      if(!mouseIsDown) {
        p1.Set(0,0);
        p2.Set(0,0);
      }
    }

    if((double)kinematicBodies[0]->GetPosition().y < -1.5) kinematicBodies[0]->SetLinearVelocity(b2Vec2(0, 1));
    if((double)kinematicBodies[0]->GetPosition().y > 0.5) kinematicBodies[0]->SetLinearVelocity(b2Vec2(0, -1));
    if((double)kinematicBodies[1]->GetPosition().y < -1.5) kinematicBodies[1]->SetLinearVelocity(b2Vec2(0, 1));
    if((double)kinematicBodies[1]->GetPosition().y > 0.5) kinematicBodies[1]->SetLinearVelocity(b2Vec2(0, -1));
    if((double)kinematicBodies[2]->GetPosition().x < -0.5) kinematicBodies[2]->SetLinearVelocity(b2Vec2(1, 0));
    if((double)kinematicBodies[2]->GetPosition().x > 1.5) kinematicBodies[2]->SetLinearVelocity(b2Vec2(-1, 0));
    if((double)kinematicBodies[3]->GetPosition().x < -0.5) kinematicBodies[3]->SetLinearVelocity(b2Vec2(1, 0));
    if((double)kinematicBodies[3]->GetPosition().x > 1.5) kinematicBodies[3]->SetLinearVelocity(b2Vec2(-1, 0));
  }

  static Test* Create()
  {
    return new FooTest;
  }
};

#endif
