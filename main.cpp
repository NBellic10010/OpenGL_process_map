#include <stdio.h>
#include <stdlib.h>
#include <tinyxml/tinyxml.h>
#include <vector>
#include <iostream>
#include <GL/glut.h>
#include <math.h>

#include "vertex.h"
#include "Process.h"
#include "Resource.h"

std::vector<vertex*> vertices; //边
std::vector<Resource*> resourcelist; //资源
std::vector<Process*> processlist; //进程

typedef const char* CSTRING; 
void read_map_from_xml() {
    CSTRING file_path = "map.xml"; //打开map.xml
    TiXmlDocument doc; 
    if(doc.LoadFile(file_path)) doc.Print(); //载入
    else {
        std::cout << "File does not exist.\n" << std::endl;
        exit(0);
    }
    
    TiXmlElement* rootElement = doc.RootElement(); //xml根节点
    TiXmlElement* processParentElement = rootElement->FirstChildElement(); //processes节点
    TiXmlElement* processElement = processParentElement->FirstChildElement(); //process节点

    /**
     * 令表示进程的圆在窗口左侧，表示资源的方形在窗口右侧
     *  
     **/
    int px = -500, py = 500; //左上初始点
    while(processElement != NULL) {
        TiXmlAttribute* attribute = processElement->FirstAttribute(); //读取pid属性
        CSTRING pid = attribute->Value(); //读出为const char*字符串
        Process* p = new Process(atoi(pid));
        p->x = px + 10; p->y = py;//将表示进程的圆右侧的点作为箭头连线的点
        processlist.push_back(p);//放入数组以备后用
        processElement = processElement->NextSiblingElement();//寻找下一个进程
        py -= 20; //向下走20个像素点作为下一个圆的新起点
    }

    int rx = 500, ry = 500; //右上初始点
    //读取xml中资源的操作，基本与上面相同
    TiXmlElement* resourceParentElement = processParentElement->NextSiblingElement(); 
    TiXmlElement* resourceElement = resourceParentElement->FirstChildElement(); 
    while(resourceElement != NULL) {
        TiXmlAttribute* attribute = processElement->FirstAttribute();
        CSTRING rid = attribute->Value(); //读取资源Id
        attribute = attribute->Next(); 
        CSTRING amount = attribute->Value();
        Resource* r = new Resource(atoi(rid), atoi(amount));
        r->x = rx - 10; r->y = ry;
        resourcelist.push_back(r);
        resourceElement = resourceElement->NextSiblingElement();
        py -= 20;
    }

    TiXmlElement* verticesElement = resourceParentElement->NextSiblingElement();
    TiXmlElement* vertexElement = verticesElement->FirstChildElement();//读取边，也就是进程资源图中进程的申请和资源的分配操作
    while(vertexElement != NULL) {
        TiXmlAttribute* attribute = vertexElement->FirstAttribute(); 
        CSTRING rid = attribute->Value(); //读取资源号
        attribute = attribute->Next();
        CSTRING pid = attribute->Value(); //读取进程号
        attribute = attribute->Next();
        CSTRING start = attribute->Value(); //读取方向标志，0代表从进程指向资源（进程申请资源），1代表从资源指向进程（为进程分配资源）

        vertex* v = new vertex(atoi(rid), atoi(pid), atoi(start)); //创建一条边
        vertices.push_back(v); 
    }

}

//画带箭头的直线
void drawarrow(int from_x, int from_y, int to_x, int to_y) {
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);

    glVertex2d(from_x, from_y);
    glVertex2d(to_x, to_y);

    glEnd();
}

//画表示进程的圆，切圆法
void drawprocess(int x, int y) {
    float radius = 10.0f;
    const GLfloat pi = 3.1415926536f;
    int i = 0;
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
    for(int i = 0; i < 100; i++) {
        glVertex2f(radius * cos(2*pi/100*i), radius * sin(2*pi/100*i));
    }

    glEnd();
}

//画表示资源的方形
void drawresource(int x, int y) {
    glColor3d(0, 0,0);
    glBegin(GL_LINE_LOOP);

    glVertex2d(x+5, y+5);
    glVertex2d(x-5, y+5);
    glVertex2d(x+5, y-5);
    glVertex2d(x-5, y-5);

    glEnd();

}

//display方法，是opengl展示窗口的回调方法
void display() {
    glClearColor(1, 1, 1, 0.0); //设置窗口背景颜色：黑色
    glClear(GL_COLOR_BUFFER_BIT);
    int i;

    //画进程
    for(i = 0; i < processlist.size(); i++) {
        Process* process = processlist[i];
        drawprocess(process->x, process->y);
    }

    //画资源
    for(i = 0; i < resourcelist.size(); i++) {
        Resource* resource = resourcelist[i];
        drawresource(resource->x, resource->y);
    }

    //画箭头
    for(i = 0; i < vertices.size(); i++) {
        vertex* v = vertices[i];
        Process* p = processlist[v->pid];
        Resource* r = resourcelist[v->rid];
        if(v->start) drawarrow(p->x, p->y, r->x, r->y);
        else drawarrow(r->x, r->y, p->x, p->y);
    }

    //放在最后的函数，将所有的gl操作送入cpu执行
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                 // 初始化 GLUT
   glutCreateWindow("OpenGL Setup Test"); // 创建给定标题名的窗口
   glutInitWindowSize(1440, 900);   // 设置初始宽高
   glutInitWindowPosition(50, 50); // 窗口距屏幕右上角的距离
   glutDisplayFunc(display); // 回调函数，执行display()函数
   glutMainLoop();           // 死循环
   return 0;
}