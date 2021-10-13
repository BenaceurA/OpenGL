#include "Framebuffer.h"
#include <glad/glad.h>
#include <iostream>

Framebuffer::Framebuffer()

{
    
}

void Framebuffer::init(int width,int height)
{
    this->width = width;
    this->height = height;

    glGenTextures(1, &colorAttachement);
    glBindTexture(GL_TEXTURE_2D, colorAttachement);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &depthStencilAttachement);
    glBindRenderbuffer(GL_RENDERBUFFER, depthStencilAttachement);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachement, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilAttachement);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "framebuffer incomplete";
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    hasColor = true;
    hasDepth = true;
    hasStencil = true;
}

void Framebuffer::initDepth(int width, int height)
{
    this->width = width;
    this->height = height;

    glGenTextures(1, &depthAttachement);
    glBindTexture(GL_TEXTURE_2D, depthAttachement);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachement, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "framebuffer incomplete";
    }
}

void Framebuffer::bindDefault()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

unsigned int Framebuffer::getColorAttachement()
{
    return colorAttachement;
}

unsigned int Framebuffer::getDepthStencilAttachement()
{
    return depthStencilAttachement;
}

unsigned int Framebuffer::getDepthAttachement()
{
    return depthAttachement;
}

unsigned int Framebuffer::getStencilAttachement()
{
    return stencilAttachement;
}
