#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdlib>

bool isSummer = true;

struct Cloud { float x, y; };
struct Bird { float x, y, speed; };
struct Snowflake { float x, y, speed; };
struct Car { float x, y, speed; };

std::vector<Cloud> clouds;
std::vector<Bird> birds;
std::vector<Snowflake> snowflakes;
Car car;

float sunRayAngle = 0.0f;

void initScene() {
    srand(time(0));
    clouds.clear();
    birds.clear();
    snowflakes.clear();

    for (int i = 0; i < 5; i++)
        clouds.push_back({float(rand() % 800), float(350 + rand() % 30)});

    for (int i = 0; i < 7; i++)
        birds.push_back({float(rand() % 800), float(300 + rand() % 100), 1.0f + float(rand() % 50) / 100});

    for (int i = 0; i < 100; i++)
        snowflakes.push_back({float(rand() % 800), float(rand() % 500), 1.0f + float(rand() % 100) / 50});

    car = {0, 70, 3.0f};  // <-- Car Y-position updated to center on road
}

void drawCircle(float cx, float cy, float r, int segments = 100) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * 3.14159f * i / segments;
        glVertex2f(cx + cos(angle) * r, cy + sin(angle) * r);
    }
    glEnd();
}

void drawHouse() {
    glColor3f(0.8f, 0.5f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(300, 100); glVertex2f(400, 100); glVertex2f(400, 180); glVertex2f(300, 180);
    glEnd();

    glColor3f(0.5f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLES);
    glVertex2f(300, 180); glVertex2f(400, 180); glVertex2f(350, 230);
    glEnd();

    glColor3f(0.3f, 0.15f, 0.05f);
    glBegin(GL_QUADS);
    glVertex2f(340, 100); glVertex2f(360, 100); glVertex2f(360, 140); glVertex2f(340, 140);
    glEnd();

    glColor3f(0.9f, 0.9f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(310, 130); glVertex2f(330, 130); glVertex2f(330, 150); glVertex2f(310, 150);
    glEnd();
}

void drawCloud(float x, float y) {
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(x, y, 20);
    drawCircle(x + 25, y + 10, 25);
    drawCircle(x + 50, y, 20);
    drawCircle(x + 25, y - 10, 22);
}

void drawBird(const Bird &b) {
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(b.x, b.y); glVertex2f(b.x - 10, b.y + 5);
    glVertex2f(b.x, b.y); glVertex2f(b.x + 10, b.y + 5);
    glEnd();
}

void drawSunRays(float cx, float cy, float innerRadius, float outerRadius, int rayCount) {
    glColor3f(1.0f, 0.9f, 0.0f);
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glRotatef(sunRayAngle, 0, 0, 1);
    for (int i = 0; i < rayCount; ++i) {
        float angle = 2.0f * 3.14159f * i / rayCount;
        float xInner = cos(angle) * innerRadius;
        float yInner = sin(angle) * innerRadius;
        float xOuter = cos(angle) * outerRadius;
        float yOuter = sin(angle) * outerRadius;
        glBegin(GL_TRIANGLES);
        glVertex2f(xInner * 0.7f, yInner * 0.7f);
        glVertex2f(xInner, yInner);
        glVertex2f(xOuter, yOuter);
        glEnd();
    }
    glPopMatrix();
}

void drawCar(float x, float y) {
    // Car body
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glBegin(GL_QUADS);
    glVertex2f(x, y); glVertex2f(x + 60, y); glVertex2f(x + 60, y + 20); glVertex2f(x, y + 20);
    glEnd();

    // Wheels
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(x + 15, y, 7, 20);
    drawCircle(x + 45, y, 7, 20);
}

void drawRoad() {
    glColor3f(0.25f, 0.25f, 0.25f);
    glBegin(GL_QUADS);
    glVertex2f(0, 80); glVertex2f(800, 80); glVertex2f(800, 60); glVertex2f(0, 60);
    glEnd();

    // Center dashed white line
    glColor3f(1.0f, 1.0f, 1.0f);
    float dashLength = 20.0f;
    float gapLength = 15.0f;
    float startX = 0.0f;
    while (startX < 800) {
        glBegin(GL_QUADS);
        glVertex2f(startX, 72); glVertex2f(startX + dashLength, 72);
        glVertex2f(startX + dashLength, 68); glVertex2f(startX, 68);
        glEnd();
        startX += dashLength + gapLength;
    }
}

void drawSummerScene() {
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.8f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0, 100); glVertex2f(800, 100); glVertex2f(800, 0); glVertex2f(0, 0);
    glEnd();

    drawRoad();
    car.x += car.speed;
    if (car.x > 800) car.x = -60;
    drawCar(car.x, car.y);

    for (auto& c : clouds) drawCloud(c.x, c.y);

    drawSunRays(700, 450, 40, 70, 12);
    glColor3f(1.0f, 1.0f, 0.0f);
    drawCircle(700, 450, 40);

    drawHouse();

    // Tree
    glColor3f(0.55f, 0.27f, 0.07f);
    glBegin(GL_QUADS);
    glVertex2f(150, 100); glVertex2f(170, 100); glVertex2f(170, 200); glVertex2f(150, 200);
    glEnd();
    glColor3f(0.0f, 0.5f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(160, 250); glVertex2f(120, 200); glVertex2f(200, 200);
    glEnd();

    for (auto &b : birds) {
        b.x += b.speed;
        if (b.x > 800) b.x = 0;
        drawBird(b);
    }
}

void drawWinterScene() {
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.94f, 0.97f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(0, 100); glVertex2f(800, 100); glVertex2f(800, 0); glVertex2f(0, 0);
    glEnd();

    drawRoad();
    car.x += car.speed;
    if (car.x > 800) car.x = -60;
    drawCar(car.x, car.y);

    drawHouse();

    // Tree trunk and branches
    glColor3f(0.3f, 0.2f, 0.1f);
    glBegin(GL_LINES);
    glVertex2f(150, 100); glVertex2f(150, 200);
    glVertex2f(150, 180); glVertex2f(130, 210);
    glVertex2f(150, 180); glVertex2f(170, 210);
    glEnd();

    // Smaller, lighter snowman
    glColor3f(0.95f, 0.98f, 1.0f);
    drawCircle(600, 110, 18);
    drawCircle(600, 138, 13);
    drawCircle(600, 158, 9);

    // Eyes
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(595, 160, 1.2f);
    drawCircle(605, 160, 1.2f);

    // Nose
    glColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(600, 155); glVertex2f(600, 153); glVertex2f(608, 154);
    glEnd();

    // Arms
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(587, 138); glVertex2f(565, 150);
    glVertex2f(565, 150); glVertex2f(570, 155);
    glVertex2f(613, 138); glVertex2f(635, 150);
    glVertex2f(635, 150); glVertex2f(630, 155);
    glEnd();

    // Snowflakes
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(2);
    glBegin(GL_POINTS);
    for (auto &f : snowflakes) {
        f.y -= f.speed;
        if (f.y < 0) {
            f.y = 500;
            f.x = rand() % 800;
        }
        glVertex2f(f.x, f.y);
    }
    glEnd();
}

void display() {
    if (isSummer) drawSummerScene();
    else drawWinterScene();
    glutSwapBuffers();
}

void timer(int value) {
    if (isSummer) {
        sunRayAngle += 0.5f;
        if (sunRayAngle > 360) sunRayAngle -= 360;
    }
    glutPostRedisplay();
    glutTimerFunc(30, timer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 't' || key == 'T') {
        isSummer = !isSummer;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 500);
    glutCreateWindow("Summer vs Winter Landscape");
    gluOrtho2D(0, 800, 0, 500);
    initScene();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}

