#pragma once

class GraphicsManager
{
	public:
		GraphicsManager();
		~GraphicsManager();

		void LoadShaders();
		
		void Update();
		void DrawWorld(bool drawTexture);



	public:
		int worldWidth;
		int worldHeight;
		int worldNumberOfLayers;

	private:
	private:

};