#include "Body.h"
#include "../utils/Utils.h"

#include <rapidxml.hpp>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <Box2D/Common/b2Math.h>

using namespace std;
using namespace rapidxml;

void Body::loadFixturesFromFile(const char *path, float scale, float density) {
    xml_document<> doc;
    xml_node<> *root_node;
    // Read the xml file into a vector
    ifstream theFile(path);
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    // Parse the buffer using the xml file parsing library into doc
    doc.parse<0>(&buffer[0]);
    // Find our root node
    root_node = doc.first_node("bodydef");
    xml_node<> *fixtureNode = root_node->first_node("bodies")->first_node("body")->first_node("fixtures")->first_node("fixture");

    // Load fixture parameters
    float friction;
    float restitution;
    if (density == 0) {
        sscanf(fixtureNode->first_attribute("density")->value(), "%f", &density);
    }
    sscanf(fixtureNode->first_node("friction")->value(), "%f", &friction);
    sscanf(fixtureNode->first_node("restitution")->value(), "%f", &restitution);

    xml_node<> *polyNodes = fixtureNode->first_node("polygons");
    int i = 0;
    for (xml_node<> *polyNode = polyNodes->first_node("polygon"); polyNode; polyNode = polyNode->next_sibling()) {
        string str = polyNode->value();
        // Remove spaces
        str.erase(remove(str.begin(), str.end(), ' '), str.end());
        // Iterates over words between commas and set polygon vertices
        char *tok = strtok(&str[0], ",");
        int index = 0;
        float buffX = 0;
        float buffY;
        auto *polygon = new b2Vec2[8];
        int polySize = 0;
        while (tok != nullptr) {
            if (index % 2 == 0) {
                sscanf(tok, "%f", &buffX);
                buffX *= scale / sfdd::SCALE;
            } else {
                sscanf(tok, "%f", &buffY);
                buffY *= - scale / sfdd::SCALE;
                polygon[polySize] = b2Vec2(buffX, buffY);
                polySize++;
            }
            index++;
            tok = strtok(nullptr, ",");
        }
        // Configure and add fixture to body
        b2PolygonShape shape;
        shape.Set(polygon, polySize);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = density;
        fixtureDef.friction = friction;
        fixtureDef.restitution = restitution;
        body->CreateFixture(&fixtureDef);

        i++;
    }
}