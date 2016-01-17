#include "SFML/OpenGL.hpp"
#include "SFML/Graphics.hpp"
#include "Voronoi.h"
#include <array>
#include <cmath>
#include <sstream>
#include "main.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Voronoi Diagram visualizer", sf::Style::Close | sf::Style::Titlebar);
	
	int nPoints = 1000;
	auto voronoi = new Voronoi(Rect(0.f, 0.f, window.getSize().x, window.getSize().y));
	voronoi->ReserveMemory(nPoints);
	for (int i = 0; i < nPoints; i++)
		voronoi->InsertPoint(Vec2(fmod(rand(), 1280), fmod(rand(), 720)));

	sf::RectangleShape pointShape(sf::Vector2f(4, 4));
	sf::VertexArray lineShape(sf::Lines, 2);

	pointShape.setOrigin(pointShape.getSize() * 0.5f);

	sf::Vector2f mousePos(sf::Mouse::getPosition(window));
	QuadEdge* eCurrent = &voronoi->Edges().front().e[0];

	sf::Time prevTimeAvg;

	while (window.isOpen())
	{
		mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::MouseButtonPressed:

				switch (event.mouseButton.button)
				{
				case sf::Mouse::Left:
					voronoi->InsertPoint(mousePos);
					break;
				case sf::Mouse::Right:
					
					break;
				case sf::Mouse::Middle:
					break;
				}

				break;
			}
		}
		
		auto timer = sf::Clock();

		voronoi->Clear();
		for (int i = 0; i < nPoints; i++)
			voronoi->InsertPoint(Vec2(fmod(rand(), 1280), fmod(rand(), 720)));

		sf::Time timeElapsed = timer.getElapsedTime();


		//calculate exponential moving average
		if (prevTimeAvg.asMicroseconds() == 0)
			prevTimeAvg = timeElapsed;
		float timeAvgFactor = 1.0f; //0~1
		sf::Time timeAvg = timeElapsed*timeAvgFactor + prevTimeAvg*(1 - timeAvgFactor);
		prevTimeAvg = timeAvg;
		
		std::ostringstream ss;
		ss << timeAvg.asSeconds();
		window.setTitle(ss.str());


		window.clear();

		/*for (auto& v : voronoi->Vertices())
		{
			pointShape.setPosition(sf::Vector2f(v->p.x, v->p.y));
			pointShape.setFillColor(sf::Color(0xF56E77FF));
			window.draw(pointShape);
		}*/

		glLineWidth(.5f);
		//range based for doesn't work.
		//for (auto& e : voronoi->Edges_l())
		for (auto it = voronoi->Edges().begin(); it != voronoi->Edges().end(); it++)
		{
			Edge& e = *it;
			/*for (int i = 0; i < 2; i++)
			{
				lineShape[0].position = e->Org()->p;
				lineShape[1].position = (e->Org()->p + e->Dest()->p)*0.5f;
				//for (int i = 0; i < 2; i++) 
				//	lineShape[i].position.y = static_cast<float>(window.getSize().y) - lineShape[i].position.y;
			
				lineShape[0].color = sf::Color(
					//(voronoi->LocateTriangleEdge(mousePos) == e) 
					false ? 0xFF9900FF :
					0xFF0000FF);
				lineShape[1].color = sf::Color((i == 1) ? 0x444444FF : 0xFFFFFFFF);
				window.draw(lineShape);
				e = e->Sym();
			}*/
			lineShape[0].position = e.Org()->p;
			lineShape[1].position = e.Dest()->p;
			for (int i = 0; i < 2; i++)
				lineShape[i].color = sf::Color(0xFF9900FF);
			window.draw(lineShape);
		}

		window.display();
	}


	delete voronoi;


	return 0;
}