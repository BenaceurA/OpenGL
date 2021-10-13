#pragma once
class Framebuffer
{
public:
	Framebuffer();

	//initialize the framebuffer with a COLOR attachement for read/write and DEPTH STENCIL attachement for write
	void init(int width, int height);

	//initialize the framebuffer with a read/write DEPTH attachement only
	void initDepth(int width ,int height);

	static void bindDefault();
	void bind();
	unsigned int getColorAttachement();
	unsigned int getDepthStencilAttachement();
	unsigned int getDepthAttachement();
	unsigned int getStencilAttachement();
private:
	int width;
	int height;
	unsigned int colorAttachement;
	bool hasColor;
	unsigned int depthAttachement;
	bool hasDepth;
	unsigned int stencilAttachement;
	bool hasStencil;
	unsigned int depthStencilAttachement;
	unsigned int fbo;
};

