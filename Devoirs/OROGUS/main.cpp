#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include "Camera/Camera.h"
#include "Controller/Mouse.h"
#include "ResourcesManager/ResourcesManager.h"
#include "Scene/Gizmo.h"
#include "Scene/Object3D.h"
#include "Scene/Scene.h"
#include "Scene/SceneLoader.h"
#include "Utilities/Transforms.h"
#include "Utilities/Units.h"
#include "Utilities/Vectors.h"

#include <iostream>

enum class Mode
{
	Camera,
	Transformation
};

enum class TransformationType
{
	Translation,
	Rotation,
	Scale,
	Shear
};

enum class Axe
{
	X,
	Y,
	Z,
	All
};

MouseController mouse;

Scene* scene = nullptr;
Gizmo* sceneGizmo = nullptr;

Speed cameraSpeed = Speed(2.5);
auto cameraRotationSpeed = Degree(50) / Second(1);
Speed translationSpeed = Speed(2.5);
auto rotationSpeed = Degree(50) / Second(1);
auto scaleSpeed = Real(1.5) / Second(1);
auto shearSpeed = Real(0.5f) / Second(1);

bool normalVisible = false;
bool gizmoVisible = true;

Mode engineMode = Mode::Camera;
TransformationType transformationType = TransformationType::Translation;

int main()
{
    // Prototypes des fonctions
    void error_callback(int error, const char* description);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void processInput(GLFWwindow *window, Second elapsedTime);
	void showMenuOptions();

    // Initialise le callback pour la gestion des erreurs de GLFW
	glfwSetErrorCallback(error_callback);

    // Initialise GLFW
	if (!glfwInit())
	{
        // Si une erreur se produit, quitte le programme
		std::cout << "Impossible d'initialiser GLFW correctement..." << std::endl;
		std::cout << "Appuyez sur une touche pour terminer..." << std::endl;
        char c;
        std::cin >> c;
		return -1;
	}

	GLFWwindow* window;

    // On requiert OpenGL 4.1 ou plus pour fonctionner
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // MSAA x4
    glfwWindowHint(GLFW_SAMPLES, 4);

    // On cree la fenetre pour l'affichage avec GLFW
	window = glfwCreateWindow(800, 600, "IMN401 - Infographie et jeu video", nullptr, nullptr);
	if (!window)
	{
        // Si on n'est pas capable, on quitte le programme
		std::cout << "Impossible d'initialiser la fenetre de rendu correctement..." << std::endl;
		glfwTerminate();
		std::cout << "Appuyez sur une touche pour terminer..." << std::endl;
		char c;
        std::cin >> c;
		return -1;
	}

    // Assigne les callbacks pour la gestion des inputs (clavier et souris)
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialise GLEW pour pouvoir accéder aux fonctions OpenGL
	glfwMakeContextCurrent(window);	
    glewExperimental = true;
	if (GLEW_OK != glewInit())
	{
        // Si une erreur se produit, on quitte le programme
		std::cout << "Impossible d'initialiser GLEW..." << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		std::cout << "Appuyez sur une touche pour terminer..." << std::endl;
        char c;
        std::cin >> c;
		return -1;
	}

    // Initialise les gestionnaires de ressources
    ResourcesManager::Initialize();
    
    // Chargement de la scene. Pour changer la scene a charger, 
    // il faut modifier le deuxieme parametre de la methode LoadScene
    std::cout << "Chargement de la scene..." << std::endl;
    scene = SceneLoader::LoadScene("Scenes/", "Scene.scn");
    if (scene == nullptr)
    {
        // Si la scene est invalide ou introuvable, on quitte le programme
        std::cout << "Impossible de charger la scene..." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
		std::cout << "Appuyez sur une touche pour terminer..." << std::endl;
		char c;
		std::cin >> c;
        return -1;
    }
    std::cout << "Chargement termine." << std::endl;

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_MULTISAMPLE);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    scene->getCamera().ratio() = width / (float)height;
    glViewport(0, 0, width, height);

    scene->getCamera().setCameraMode(false);

    // La couleur d'effacement est un petit peu plus pale que noir 
    // pour voir les objets noirs s'il y a des problemes
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glLineWidth(4.0f);

	showMenuOptions();

	// Crée le repere dans la scene
	sceneGizmo = new Gizmo();

    // Boucle de rendu principal
    Second lastFrame = Second((float)glfwGetTime());
	Second elapsedTime = Second(0);
    while (!glfwWindowShouldClose(window))
	{
        Second currentFrame = Second((float)glfwGetTime());
        elapsedTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, elapsedTime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene->render();

		if (normalVisible)
		{
			scene->renderNormals();
		}

		if (gizmoVisible)
		{
			sceneGizmo->render(*scene);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
    // Libere les ressources utilisees
	delete sceneGizmo;
    delete scene;

    ResourcesManager::Uninitialize();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void showMenuOptions()
{
	std::cout << "==================================================================================" << std::endl;
	std::cout << "Operations possibles :" << std::endl;
	std::cout << "   Raccourcis clavier" << std::endl;
	std::cout << "      1 : Mode polygone plein" << std::endl;
	std::cout << "      2 : Mode polygone vide (wireframe)" << std::endl;
	std::cout << "      3 : Mode camera troisieme personne" << std::endl;
	std::cout << "      4 : Mode camera premiere personne" << std::endl;
	std::cout << "      5 : Mode Transformation" << std::endl;
	std::cout << "      R : Recharge la scene" << std::endl;
	std::cout << "      N : Affiche/Cache les normales des objets" << std::endl;
	std::cout << "      G : Affiche/Cache le repere de la scene" << std::endl;
	std::cout << "      L : Affiche/Cache les lumieres" << std::endl;
	std::cout << "      H : Affiche ce menu" << std::endl << std::endl;
	std::cout << "      Les touches suivantes dependent du mode courant (3, 4 ou 5)" << std::endl;
	std::cout << "        Mode 3 et 4" << std::endl;
	std::cout << "          ESC : Reinitialise la camera" << std::endl;
	std::cout << "          W : Avance la camera" << std::endl;
	std::cout << "          A : Deplace la camera vers la gauche" << std::endl;
	std::cout << "          S : Recule la camera" << std::endl;
	std::cout << "          D : Deplace la camera vers la droite" << std::endl;
	std::cout << "          Fleche gauche : Tourne la camera vers la gauche" << std::endl;
	std::cout << "          Fleche haut : Tourne la camera vers le haut" << std::endl;
	std::cout << "          Fleche droite : Tourne la camera vers la droite" << std::endl;
	std::cout << "          Fleche bas : Tourne la camera vers le bas" << std::endl;
	std::cout << "        Mode 5 " << std::endl;
	std::cout << "          ESC : Sortir du mode transformation" << std::endl;
	std::cout << "          + : Selectionne le prochain objet dans la scene" << std::endl;
	std::cout << "          - : Selectionne l'objet precedent dans la scene" << std::endl;
	std::cout << "          1 : Effectue des translations" << std::endl;
	std::cout << "             W : Applique une translation sur l'axe des Y+" << std::endl;
	std::cout << "             A : Applique une translation sur l'axe des X-" << std::endl;
	std::cout << "             X : Applique une translation sur l'axe des Y-" << std::endl;
	std::cout << "             D : Applique une translation sur l'axe des X+" << std::endl;
	std::cout << "             E : Applique une translation sur l'axe des Z+" << std::endl;
	std::cout << "             Z : Applique une translation sur l'axe des Z-" << std::endl;
	std::cout << "          2 : Effectue des rotations" << std::endl;
	std::cout << "             W : Applique une rotation positive autour de l'axe des X" << std::endl;
	std::cout << "             A : Applique une rotation negative autour de l'axe des Y" << std::endl;
	std::cout << "             X : Applique une rotation negative autour de l'axe des X" << std::endl;
	std::cout << "             D : Applique une rotation positive autour de l'axe des Y" << std::endl;
	std::cout << "             E : Applique une rotation positive autour de l'axe des Z" << std::endl;
	std::cout << "             Z : Applique une rotation negative autour de l'axe des Z" << std::endl;
	std::cout << "          3 : Effectue des mises a l'echelle" << std::endl;
	std::cout << "             W : Applique une mise a l'echelle positive sur l'axe des Y" << std::endl;
	std::cout << "             A : Applique une mise a l'echelle negative sur l'axe des X" << std::endl;
	std::cout << "             X : Applique une mise a l'echelle negative sur l'axe des Y" << std::endl;
	std::cout << "             D : Applique une mise a l'echelle positive sur l'axe des X" << std::endl;
	std::cout << "             E : Applique une mise a l'echelle positive sur l'axe des Z" << std::endl;
	std::cout << "             Z : Applique une mise a l'echelle negative sur l'axe des Z" << std::endl;
	std::cout << "             S : Applique une mise a l'echelle uniforme positive sur tous les axes" << std::endl;
	std::cout << "             C : Applique une mise a l'echelle uniforme negative sur tous les axes" << std::endl;
	std::cout << "          4 : Effectue des cisaillements" << std::endl;
	std::cout << "             W : Applique un cisaillement positif sur l'axe des Y" << std::endl;
	std::cout << "             A : Applique un cisaillement negatif sur l'axe des X" << std::endl;
	std::cout << "             X : Applique un cisaillement negatif sur l'axe des Y" << std::endl;
	std::cout << "             D : Applique un cisaillement positif sur l'axe des X" << std::endl;
	std::cout << "             E : Applique un cisaillement positif sur l'axe des Z" << std::endl;
	std::cout << "             Z : Applique un cisaillement negatif sur l'axe des Z" << std::endl << std::endl;
	std::cout << "   Souris" << std::endl;
	std::cout << "      En mode 3 et 4" << std::endl;
	std::cout << "         Bouton gauche : Tourne la camera de gauche a droite et de haut en bas" << std::endl;
	std::cout << "         Bouton droit : Deplace la camera de gauche a droite et de haut en bas" << std::endl;
	std::cout << "         Roulette : Avance et recule la camera" << std::endl;
	std::cout << "==================================================================================" << std::endl;
}

// Exécute en continue l'appui sur une touche
void processInput(GLFWwindow *window, Second elapsedTime)
{
	void processInputCamera(GLFWwindow* window, Second elapsedTime);
	void processInputTransformation(GLFWwindow* window, Second elapsedTime);

	if (engineMode == Mode::Camera)
	{
		processInputCamera(window, elapsedTime);
	}
	else
	{
		processInputTransformation(window, elapsedTime);
	}
}

void processInputCamera(GLFWwindow* window, Second elapsedTime)
{
	// Avancer la camera en appuyant sur la touche W
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		scene->getCamera().forward(cameraSpeed * elapsedTime);
	}

	// Reculer la camera en appuyant sur la touche S
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		scene->getCamera().forward(-cameraSpeed * elapsedTime);
	}

	// Bouger la camera a gauche en appuyant sur la touche A
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		scene->getCamera().strafe(Metre(-cameraSpeed * elapsedTime));
	}

	// Bouger la camera a droite en appuyant sur la touche D
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		scene->getCamera().strafe(Metre(cameraSpeed * elapsedTime));
	}

	// Tourner la camera a gauche en appuyant sur la touche Fleche gauche
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		scene->getCamera().yaw(cameraRotationSpeed * elapsedTime);
	}

	// Tourner la camera a droite en appuyant sur la touche Fleche droite
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		scene->getCamera().yaw(-cameraRotationSpeed * elapsedTime);
	}

	// Tourner la camera a gauche en appuyant sur la touche Fleche gauche
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		scene->getCamera().pitch(cameraRotationSpeed * elapsedTime);
	}

	// Tourner la camera a droite en appuyant sur la touche Fleche droite
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		scene->getCamera().pitch(-cameraRotationSpeed * elapsedTime);
	}
}

void processInputTransformation(GLFWwindow* window, Second elapsedTime)
{
	Transform executeTranslation(Axe axe, Metre delta);
	Transform executeRotation(Axe axe, Degree delta);
	Transform executeScale(Axe axe, Real delta);
	Transform executeShear(Axe axe, Real delta);

	bool keyPressed = false;
	Axe axe = Axe::X;
	Metre deltaTranslation = translationSpeed * elapsedTime;
	Degree deltaRotation = rotationSpeed * elapsedTime;
	Real deltaScale = Real(1) + scaleSpeed * elapsedTime;
	Real deltaShear = shearSpeed * elapsedTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		keyPressed = true;
		if (transformationType == TransformationType::Rotation)
		{
			axe = Axe::X;
		}
		else
		{
			axe = Axe::Y;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		keyPressed = true;
		if (transformationType == TransformationType::Rotation)
		{
			axe = Axe::Y;
		}
		else
		{
			axe = Axe::X;
		}
		deltaTranslation = -deltaTranslation;
		deltaRotation = -deltaRotation;
		deltaScale = Real(1) - scaleSpeed * elapsedTime;
		deltaShear = -deltaShear;

	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		keyPressed = true;
		if (transformationType == TransformationType::Rotation)
		{
			axe = Axe::Y;
		}
		else
		{
			axe = Axe::X;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		keyPressed = true;
		if (transformationType == TransformationType::Rotation)
		{
			axe = Axe::X;
		}
		else
		{
			axe = Axe::Y;
		}
		deltaTranslation = -deltaTranslation;
		deltaRotation = -deltaRotation;
		deltaScale = Real(1) - scaleSpeed * elapsedTime;
		deltaShear = -deltaShear;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		keyPressed = true;
		if (transformationType == TransformationType::Scale)
		{
			axe = Axe::All;
		}
		else
		{
			keyPressed = false;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		keyPressed = true;
		if (transformationType == TransformationType::Scale)
		{
			axe = Axe::All;
			deltaScale = Real(1) - scaleSpeed * elapsedTime;
		}
		else
		{
			keyPressed = false;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		keyPressed = true;
		axe = Axe::Z;
	}
	else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		keyPressed = true;
		axe = Axe::Z;
		deltaTranslation = -deltaTranslation;
		deltaRotation = -deltaRotation;
		deltaScale = Real(1) - scaleSpeed * elapsedTime;
		deltaShear = -deltaShear;
	}

	if (keyPressed)
	{
		Transform t = Transform();
		if (transformationType == TransformationType::Translation)
		{
			t = executeTranslation(axe, deltaTranslation);
		}
		else if (transformationType == TransformationType::Rotation)
		{
			t = executeRotation(axe, deltaRotation);
		}
		else if (transformationType == TransformationType::Scale)
		{
			t = executeScale(axe, deltaScale);
		}
		else if (transformationType == TransformationType::Shear)
		{
			t = executeShear(axe, deltaShear);
		}

		Object3D* selected = scene->getCurrentSelectedObject();
		if (selected != nullptr)
		{
			selected->transformObject(t);
		}
	}
}

void error_callback(int error, const char* description)
{
    std::cout << "Error (" << error << ") : " << description << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    scene->getCamera().ratio() = width / (float)height;
    glViewport(0, 0, width, height);
}

// Callback pour les appuie de touche ponctuelle
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
		if (engineMode == Mode::Camera)
		{
			scene->getCamera().reset();
			scene->getCamera().log();
		}
		else
		{
			engineMode = Mode::Camera;
			std::cout << "Mode courant : Camera" << std::endl;
		}
    }

	// Desactiver le mode Wireframe en appuyant sur la touche 1
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		if (engineMode == Mode::Camera)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			transformationType = TransformationType::Translation;
			std::cout << "Effectue des translations" << std::endl;
		}
	}

	// Activer le mode Wireframe en appuyant sur la touche 2
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		if (engineMode == Mode::Camera)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			transformationType = TransformationType::Rotation;
			std::cout << "Effectue des rotations" << std::endl;
		}
	}

	// Change le type de camera pour troisieme personne
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		if (engineMode == Mode::Camera)
		{
			scene->getCamera().setCameraMode(false);
		}
		else
		{
			transformationType = TransformationType::Scale;
			std::cout << "Effectue des mises a l'echelle" << std::endl;
		}
	}

	// Change le type de camera pour première personne
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		if (engineMode == Mode::Camera)
		{
			scene->getCamera().setCameraMode(true);
		}
		else
		{
			transformationType = TransformationType::Shear;
			std::cout << "Effectue des cisaillements" << std::endl;
		}
	}

	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		engineMode = Mode::Transformation;
		std::cout << "Mode courant : Transformations" << std::endl;
		std::cout << "Objet selectionne : " << scene->getCurrentSelectedObject()->getName() << std::endl;
	}

	// Sélectionner le prochain object dans la scene en appuyant sur la touche +
	if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
	{
		if (engineMode == Mode::Transformation)
		{
			scene->changeSelectedObject(1);
			std::cout << "Objet selectionne : " << scene->getCurrentSelectedObject()->getName() << std::endl;
		}
	}

	// Sélectionner le prochain object dans la scene en appuyant sur la touche +
	if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
	{
		if (engineMode == Mode::Transformation)
		{
			scene->changeSelectedObject(-1);
			std::cout << "Objet selectionne : " << scene->getCurrentSelectedObject()->getName() << std::endl;
		}
	}

	// Activer le mode Wireframe en appuyant sur la touche 2
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		Scene* newScene = SceneLoader::LoadScene("Scenes/", "Scene.scn");
		if (newScene == nullptr)
		{
			// Si la scene est invalide ou introuvable, on garde l'ancienne scene
			std::cout << "Impossible de charger la scene..." << std::endl;
			return;
		}
		delete scene;
		scene = newScene;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		scene->getCamera().ratio() = width / (float)height;
		scene->getCamera().setCameraMode(false);
		std::cout << "Chargement termine." << std::endl;
		showMenuOptions();
	}

	// Affiche/Cache les normales
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		normalVisible = !normalVisible;
	}

	// Affiche/Cache le repere
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		gizmoVisible = !gizmoVisible;
	}

	// Affiche/Cache le repere
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		scene->showLights(!scene->lightsVisible());
	}

	// Affiche le menu
	if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		showMenuOptions();
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    mouse.updateButtonState(button, action == GLFW_PRESS);
    if (action != GLFW_PRESS)
    {
        scene->getCamera().log();
    }
}

void mouse_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    mouse.updatePosition((float)xpos, (float)ypos);
    if (mouse.LButtonPressed())
    {
        // Avec le bouton gauche appuye, on peut tourner la camera
        // de gauche a droite et de bas en haut
        scene->getCamera().yaw(Degree(mouse.lastMovX()) * 0.1f);
        scene->getCamera().pitch(Degree(mouse.lastMovY()) * 0.1f);
    }
	else if (mouse.RButtonPressed())
	{
		// Avec le bouton droit appuye, on peut bouger la camera 
		// de haut en bas et de droite a gauche
		scene->getCamera().upDown(Metre(mouse.lastMovY() * 0.1f));
		scene->getCamera().strafe(Metre(mouse.lastMovX() * 0.1f));
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    scene->getCamera().forward(Metre((float)yoffset * 0.5f));
}

Transform executeTranslation(Axe axe, Metre delta)
{
	Vector3<Real> axeVector;
	if (axe == Axe::X)
	{
		axeVector = Vector3<Real>(1, 0, 0);
	}
	else if (axe == Axe::Y)
	{
		axeVector = Vector3<Real>(0, 1, 0);
	}
	else if (axe == Axe::Z)
	{
		axeVector = Vector3<Real>(0, 0, 1);
	}
	return Transform::MakeTranslation(axeVector * delta);
}

Transform executeRotation(Axe axe, Degree delta)
{
	// TP2 : À compléter	
	return Transform();
}

Transform executeScale(Axe axe, Real delta)
{
	// TP2 : À compléter
	return Transform();
}

Transform executeShear(Axe axe, Real delta)
{
	// TP2 : À compléter
	return Transform();
}
