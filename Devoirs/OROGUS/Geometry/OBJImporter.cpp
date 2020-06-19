#include "OBJImporter.h"

#include "Geometry.h"
#include "GeometryHelper.h"
#include "../Utilities/Logger.h"
#include "../Utilities/StringUtilities.h"
#include "../Utilities/Types.h"
#include "../Utilities/Units.h"

#include <fstream>
#include <iostream>
#include <vector>

Geometry* OBJGeometryImporter::Import(const std::string& fileName)
{
	std::ifstream fileReader(fileName);

	if (fileReader.is_open())
	{
        std::vector<Vertex> vertices;
        std::vector<uint32> indexes;

		std::vector<Point3<Metre>> positionList;
		std::vector<Vector2<Real>> texturedList;
		std::vector<Vector3<Real>> normalList;

		std::string line;
		while (!std::getline(fileReader, line))
		{
			if (StringUtilities::StartsWith(line, '#') || line.empty())
			{
				continue;
			}
			else
			{
				std::vector<std::string> values = StringUtilities::Split(line, ' ');

				if (values[0] != "g")
				{
					if (values[0] == "v")
					{
						float v1, v2, v3;

						v1 = std::stof(values[1]);
						v2 = std::stof(values[2]);
						v3 = std::stof(values[3]);

						positionList.push_back(Point3<Metre>(Metre(v1), Metre(v2), Metre(v3)));
					}
					else if (values[0] == "vt")
					{
						float v1, v2;

						v1 = std::stof(values[1]);
						v2 = std::stof(values[2]);
						
						texturedList.push_back(Vector2<Real>(v1, v2));
					}
					else if (values[0] == "vn")
					{
						float v1, v2, v3;
						v1 = std::stof(values[1]);
						v2 = std::stof(values[2]);
						v3 = std::stof(values[3]);

						normalList.push_back(Vector3<Real>(v1, v2, v3));
					}
					else if (values[0] == "f")
					{
						for (int i = 1; i < 4; ++i)
						{
							std::vector<std::string> tripletValue = StringUtilities::Split(values[i], '/');
							int position, texture, normal;
							position = std::stoi(tripletValue[0]);
							texture = std::stoi(tripletValue[1]);
							normal = std::stoi(tripletValue[2]);

							if (position > 0 && (uint32)position <= (uint32)positionList.size() && texture > 0 && (uint32)texture <= (uint32)texturedList.size() && normal > 0 && (uint32)normal <= (uint32)normalList.size())
							{
								Point3<Metre> vPos = positionList[position - 1];
								Vector3<Real> vNormal = normalList[normal - 1];
								Vector2<Real> vUV = texturedList[texture - 1];

                                Vertex v = Vertex(vPos, vNormal, vUV);

                                uint32 index = 0;
                                auto it = std::find(vertices.begin(), vertices.end(), v);
                                if (it == vertices.end())
                                {
                                    vertices.push_back(v);
                                    index = (uint32)vertices.size() - 1;
                                }
                                else
                                {
                                    index = (uint32)std::distance(vertices.begin(), it);
                                }

                                indexes.push_back(index);
							}
							else
							{
								Log() << "--Erreur : La face contient des numeros de vertex invalide : " << values[i] << std::endl;
								return nullptr;
							}
						}
					}
				}
			}
		}
        return Geometry::CreateGeometry(fileName, std::move(vertices), std::move(indexes));
	}
    else
    {
        Log() << "-- Erreur : Impossible d'ouvrir le fichier " << fileName << std::endl;
        return nullptr;
    }

	
}