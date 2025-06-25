#include <iostream>
#include <GL/freeglut.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;
// Power-up variables
bool powerUpActive = false;
float powerUpX = 0.0f, powerUpY = 0.0f;
bool powerUpShot = false;


bool SpongBulletActive = false;
bool SpongPowerShot = false;
float SpongBulletX = 0.0f, SpongBulletY = 0.0f;
float SpongBulletSpeed = 0.05f;
int SpongLives = 5;

float charX = 0.0f, charY = -0.8f;
float charWidth = 0.1f, charHeight = 0.2f;
int charLives = 5;

float bulletX = 0.0f, bulletY = -1.0f;
bool bulletActive = false;
float bulletSpeed = 0.02f;

float mrKrabsX = 0.5f, mrKrabsY = 0.8f;
float mrKrabsWidth = 0.15f, mrKrabsHeight = 0.25f;
int mrKrabsLives = 5;
bool mrKrabsHit = false;
bool mrKrabsPowerUpShot = false;
float mrKrabsBulletX = mrKrabsX, mrKrabsBulletY = mrKrabsY;
bool mrKrabsBulletActive = false;
float mrKrabsBulletSpeed = -0.02f;

bool gameOver = false;
bool playerWon = false;

bool checkCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
    return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

void renderText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}
bool checkCollision(float bulletX, float bulletY, float mrKrabsX, float mrKrabsY, float mrKrabsWidth, float mrKrabsHeight) {
    return bulletX > mrKrabsX - mrKrabsWidth / 2 && bulletX < mrKrabsX + mrKrabsWidth / 2 &&
        bulletY > mrKrabsY - mrKrabsHeight / 2 && bulletY < mrKrabsY + mrKrabsHeight / 2;
}


// Render Mr. Krabs bullet
void renderMrKrabsBullet() {
    if (mrKrabsBulletActive) {
        glColor3f(0.0f, 1.0f, 1.0f); // Bullet color (cyan)
        glBegin(GL_QUADS);
        glVertex2f(mrKrabsBulletX - 0.01f, mrKrabsBulletY - 0.02f);
        glVertex2f(mrKrabsBulletX + 0.01f, mrKrabsBulletY - 0.02f);
        glVertex2f(mrKrabsBulletX + 0.01f, mrKrabsBulletY + 0.02f);
        glVertex2f(mrKrabsBulletX - 0.01f, mrKrabsBulletY + 0.02f);
        glEnd();
    }
}


// Move Mr. Krabs (random movement)
void moveMrKrabs() {
    float moveX = (rand() % 3 - 1) * 0.05f;
    float moveY = (rand() % 3 - 1) * 0.05f;
    mrKrabsX += moveX;
    mrKrabsY += moveY;

    if (mrKrabsX < -0.9f) mrKrabsX = -0.9f;
    if (mrKrabsX > 0.9f) mrKrabsX = 0.9f;
    if (mrKrabsY < -0.5f) mrKrabsY = -0.5f;
    if (mrKrabsY > 0.9f) mrKrabsY = 0.9f;

    // Handle bullet firing
    if (!mrKrabsBulletActive) {
        mrKrabsBulletX = mrKrabsX;
        mrKrabsBulletY = mrKrabsY - mrKrabsHeight / 2;
        mrKrabsBulletActive = true;
    }
}
float SpongX = -0.3f, SpongY = -0.8f;
float SpongWidth = 0.1f, SpongHeight = 0.2f;

void display() {

    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    glColor3f(0.68f, 0.85f, 0.90f);
    glVertex2f(-1.0f, 1.0f);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glEnd();
    glLoadIdentity();

    if (gameOver) {
        if (playerWon) {
            glColor3f(0.13f, 0.55f, 0.13f);
            renderText(-0.2f, 0.0f, "You Won!");
        }
        else {
            glColor3f(0.96f, 0.80f, 0.69f);
            renderText(-0.2f, 0.0f, "You Lost!");
        }
        glutSwapBuffers();
        return;
    }

    if (SpongBulletActive) {
        glPushMatrix();
        glColor3f(SpongPowerShot ? 1.0f : 0.0f, 0.5f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(SpongBulletX - 0.01f, SpongBulletY + 0.02f);
        glVertex2f(SpongBulletX + 0.01f, SpongBulletY + 0.02f);
        glVertex2f(SpongBulletX + 0.01f, SpongBulletY - 0.02f);
        glVertex2f(SpongBulletX - 0.01f, SpongBulletY - 0.02f);
        glEnd();
        glPopMatrix();
    }
    // Draw Power-up (if active)
    if (powerUpActive) {
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for power-up
        glBegin(GL_QUADS);
        glVertex2f(powerUpX - 0.02f, powerUpY - 0.02f);
        glVertex2f(powerUpX + 0.02f, powerUpY - 0.02f);
        glVertex2f(powerUpX + 0.02f, powerUpY + 0.02f);
        glVertex2f(powerUpX - 0.02f, powerUpY + 0.02f);
        glEnd();
    }
    // Sky bubbles (stars)
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 100; i++) { // Increased from 50 to 100
        float x = ((rand() % 300) - 150) / 100.0f; // Wider range for x
        float y = ((rand() % 200) + 50) / 100.0f;  // Wider range for y
        glVertex2f(x, y);
    }
    glEnd();


    glColor3f(1.0f, 0.9f, 0.3f);  // Soft yellow color


    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);  // Sand at the bottom
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, -0.5f);
    glVertex2f(-1.0f, -0.5f);
    glEnd();

    // Draw SpongePop's House

        // Base of the house (cylindrical body)
    glColor3f(1.0f, 0.6f, 0.0f); // Orange color
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14159f / 180;
        glVertex2f(0.7f + 0.15f * cos(angle), -0.3f + 0.3f * sin(angle));
    }
    glEnd();

    // Door
    glColor3f(0.4f, 0.2f, 0.0f); // Brown door
    glBegin(GL_POLYGON);
    glVertex2f(0.65f, -0.5f);
    glVertex2f(0.75f, -0.5f);
    glVertex2f(0.75f, -0.3f);
    glVertex2f(0.65f, -0.3f);
    glEnd();

    // Windows
    glColor3f(0.6f, 0.8f, 1.0f);


    glBegin(GL_POLYGON);
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14159f / 180;
        glVertex2f(0.65f + 0.05f * cos(angle), -0.2f + 0.05f * sin(angle));
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14159f / 180;
        glVertex2f(0.75f + 0.05f * cos(angle), -0.2f + 0.05f * sin(angle));
    }
    glEnd();

    // Roof (green leaves)
    glColor3f(0.0f, 0.8f, 0.0f); // Green color
    glBegin(GL_TRIANGLES);
    glVertex2f(0.55f, -0.05f); // Left leaf (lowered)
    glVertex2f(0.7f, 0.3f);    // Top point (unchanged)
    glVertex2f(0.85f, -0.05f); // Right leaf (lowered)
    glEnd();



    glPushMatrix();
    glTranslatef(SpongX, SpongY, 0.0f);

    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    // for SpongeBob's body
    glBegin(GL_QUADS);
    glVertex2f(-0.08f, 0.15f);
    glVertex2f(0.08f, 0.15f);
    glVertex2f(0.08f, -0.05f);
    glVertex2f(-0.08f, -0.05f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.08f, -0.05f);
    glVertex2f(0.08f, -0.05f);
    glVertex2f(0.08f, -0.09f);
    glVertex2f(-0.08f, -0.09f);
    glEnd();

    // Draw a smile
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for the smile
    glPushMatrix();
    glTranslatef(0.0f, 0.04f, 0.0f); // Adjust smile position
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 180; i++) {
        float angle = i * 3.14159f / 180; // Convert degree to radians
        float x = 0.05f * cos(angle);    // Radius for the arc (width of smile)
        float y = -0.02f * sin(angle);   // Height of the arc
        glVertex2f(x, y);
    }
    glEnd();
    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    glVertex2f(-0.015f, 0.015f);
    glVertex2f(-0.005f, 0.015f);
    glVertex2f(-0.005f, 0.005f);
    glVertex2f(-0.015f, 0.005f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(0.005f, 0.015f);
    glVertex2f(0.015f, 0.015f);
    glVertex2f(0.015f, 0.005f);
    glVertex2f(0.005f, 0.005f);
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, -0.05f);
    glVertex2f(-0.02f, -0.08f);
    glVertex2f(0.02f, -0.08f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(-0.01f, -0.08f);
    glVertex2f(0.01f, -0.08f);
    glVertex2f(0.01f, -0.12f);
    glVertex2f(-0.01f, -0.12f);
    glEnd();

    glColor3f(0.6f, 0.3f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.08f, -0.09f);
    glVertex2f(0.08f, -0.09f);
    glVertex2f(0.08f, -0.14f);
    glVertex2f(-0.08f, -0.14f);
    glEnd();

    glColor3f(1.0f, 0.9f, 0.4f);
    glBegin(GL_QUADS);

    glVertex2f(-0.04f, -0.14f);
    glVertex2f(-0.02f, -0.14f);
    glVertex2f(-0.02f, -0.20f);
    glVertex2f(-0.04f, -0.20f);

    glVertex2f(0.02f, -0.14f);
    glVertex2f(0.04f, -0.14f);
    glVertex2f(0.04f, -0.20f);
    glVertex2f(0.02f, -0.20f);

    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(-0.08f, 0.05f);
    glVertex2f(-0.15f, -0.05f);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(0.08f, 0.05f);
    glVertex2f(0.15f, -0.05f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    GLUquadric* quadricEye = gluNewQuadric();
    if (quadricEye) {

        glPushMatrix();
        glTranslatef(-0.03f, 0.1f, 0.0f);
        gluDisk(quadricEye, 0.0f, 0.03f, 100, 1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.03f, 0.1f, 0.0f);
        gluDisk(quadricEye, 0.0f, 0.03f, 100, 1);
        glPopMatrix();

        glColor3f(0.0f, 0.0f, 0.0f);

        glPushMatrix();
        glTranslatef(-0.03f, 0.1f, 0.0f);
        gluDisk(quadricEye, 0.0f, 0.01f, 100, 1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.03f, 0.1f, 0.0f);
        gluDisk(quadricEye, 0.0f, 0.01f, 100, 1);
        glPopMatrix();
    }


    glPopMatrix();

    glPushMatrix();
    glTranslatef(charX, charY, 0.0f);
    glScalef(1.0f, 1.5f, 1.0f); // Scale PATRICK 

    GLUquadric* quadric = gluNewQuadric();
    if (!quadric) return;

    glColor3f(1.0f, 0.6f, 0.8f);
    glBegin(GL_TRIANGLES);

    glVertex2f(-0.08f, -0.1f);
    glVertex2f(-0.14f, -0.2f);
    glVertex2f(-0.04f, -0.2f);

    glVertex2f(0.08f, -0.1f);
    glVertex2f(0.14f, -0.2f);
    glVertex2f(0.04f, -0.2f);
    glEnd();

    glColor3f(0.2f, 0.8f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(-0.12f, -0.1f);
    glVertex2f(0.12f, -0.1f);
    glVertex2f(0.12f, -0.16f);
    glVertex2f(-0.12f, -0.16f);
    glEnd();

    glColor3f(0.6f, 0.2f, 0.8f);
    glPushMatrix();
    glTranslatef(-0.06f, -0.12f, 0.0f);
    gluDisk(quadric, 0.0f, 0.02f, 100, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.06f, -0.12f, 0.0f);
    gluDisk(quadric, 0.0f, 0.02f, 100, 1);
    glPopMatrix();

    glColor3f(1.0f, 0.6f, 0.8f);
    glBegin(GL_TRIANGLES);

    glVertex2f(0.0f, 0.1f);

    glVertex2f(-0.12f, -0.1f);

    glVertex2f(0.12f, -0.1f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    glTranslatef(-0.04f, 0.05f, 0.0f);
    gluDisk(quadric, 0.0f, 0.03f, 100, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.04f, 0.05f, 0.0f);
    gluDisk(quadric, 0.0f, 0.03f, 100, 1);
    glPopMatrix();

    glColor3f(0.0f, 0.0f, 0.0f);

    glPushMatrix();
    glTranslatef(-0.04f, 0.05f, 0.0f);
    gluDisk(quadric, 0.0f, 0.01f, 100, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.04f, 0.05f, 0.0f);
    gluDisk(quadric, 0.0f, 0.01f, 100, 1);
    glPopMatrix();

    glColor3f(1.0f, 0.6f, 0.8f);

    glPushMatrix();
    glTranslatef(-0.07f, -0.07f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(-0.02f, 0.01f);
    glVertex2f(-0.05f, 0.01f);
    glVertex2f(-0.08f, -0.1f);
    glVertex2f(-0.05f, -0.1f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.07f, -0.07f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(0.02f, 0.01f);
    glVertex2f(0.05f, 0.01f);
    glVertex2f(0.08f, -0.1f);
    glVertex2f(0.05f, -0.1f);
    glEnd();
    glPopMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, -0.05f, 0.0f);

    glColor3f(0.0f, 0.0f, 0.0f); // Black color for the smile
    glPushMatrix();
    glTranslatef(0.0f, 0.02f, 0.0f); // Position the smile
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 180; i++) {
        float angle = i * 3.14159f / 180; // Convert degree to radians
        float x = 0.05f * cos(angle);    // Radius for the arc (width of smile)
        float y = -0.02f * sin(angle);   // Height of the arc
        glVertex2f(x, y);
    }
    glEnd();
    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    glTranslatef(-0.02f, -0.05f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.02f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, -0.03f);
    glVertex2f(-0.02f, -0.03f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.02f, -0.05f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.02f, 0.0f);
    glVertex2f(0.02f, -0.03f);
    glVertex2f(0.0f, -0.03f);
    glEnd();
    glPopMatrix();

    glPopMatrix();

    if (bulletActive) {
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(bulletX - 0.01f, bulletY - 0.02f);
        glVertex2f(bulletX + 0.01f, bulletY - 0.02f);
        glVertex2f(bulletX + 0.01f, bulletY + 0.02f);
        glVertex2f(bulletX - 0.01f, bulletY + 0.02f);
        glEnd();
    }

    if (mrKrabsBulletActive) {
        glColor3f(0.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2f(mrKrabsBulletX - 0.01f, mrKrabsBulletY - 0.02f);
        glVertex2f(mrKrabsBulletX + 0.01f, mrKrabsBulletY - 0.02f);
        glVertex2f(mrKrabsBulletX + 0.01f, mrKrabsBulletY + 0.02f);
        glVertex2f(mrKrabsBulletX - 0.01f, mrKrabsBulletY + 0.02f);
        glEnd();
    }


    if (!mrKrabsHit) {
        glPushMatrix();
        glTranslatef(mrKrabsX, mrKrabsY, 0.0f);

        glColor3f(1.0f, 0.0f, 0.0f); // Red color for body
        glPushMatrix();
        glScalef(1.0f, 0.8f, 1.0f); // Scale to make body smaller
        gluDisk(gluNewQuadric(), 0.0f, 0.15f, 100, 1); // Smaller size
        glPopMatrix();


        glPushMatrix();
        glTranslatef(-0.1f, -0.2f, 0.0f); // Move leg closer to the body
        glColor3f(1.0f, 0.0f, 0.0f); // Red color for leg
        glBegin(GL_QUADS);
        glVertex2f(-0.05f, 0.0f); // Bottom-left
        glVertex2f(0.05f, 0.0f);  // Bottom-right
        glVertex2f(0.05f, -0.1f); // Top-right (extend downward)
        glVertex2f(-0.05f, -0.1f); // Top-left (extend downward)
        glEnd();
        glPopMatrix();

        // Draw the right leg - Rectangle attached to body (right)
        glPushMatrix();
        glTranslatef(0.1f, -0.2f, 0.0f);  // Move closer by decreasing the Y-value
        // Move leg closer to the body
        glColor3f(1.0f, 0.0f, 0.0f); // Red color for leg
        glBegin(GL_QUADS);
        glVertex2f(-0.05f, 0.0f); // Bottom-left
        glVertex2f(0.05f, 0.0f);  // Bottom-right
        glVertex2f(0.05f, -0.1f); // Top-right (extend downward)
        glVertex2f(-0.05f, -0.1f); // Top-left (extend downward)
        glEnd();
        glPopMatrix();

        glColor3f(1.0f, 1.0f, 1.0f); // White color for eyes
        glPushMatrix();
        glTranslatef(-0.1f, 0.2f, 0.0f); // Position for left eye
        gluDisk(gluNewQuadric(), 0.0f, 0.07f, 100, 1); // Larger eye
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.1f, 0.2f, 0.0f); // Position for right eye
        gluDisk(gluNewQuadric(), 0.0f, 0.07f, 100, 1); // Larger eye
        glPopMatrix();


        // Draw the pupils (black) for larger eyes
        glColor3f(0.0f, 0.0f, 0.0f); // Black color for pupils
        glPushMatrix();
        glTranslatef(-0.1f, 0.2f, 0.0f); // Position for left pupil, adjusted for larger eyes
        gluDisk(gluNewQuadric(), 0.0f, 0.04f, 100, 1); // Larger pupil
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.1f, 0.2f, 0.0f); // Position for right pupil, adjusted for larger eyes
        gluDisk(gluNewQuadric(), 0.0f, 0.04f, 100, 1); // Larger pupil
        glPopMatrix();


        // Draw the pupils (black)


        glPushMatrix();
        glColor3f(1.0f, 0.0f, 0.0f); // Position for right pupil, lowered Y
        gluDisk(gluNewQuadric(), 0.0f, 0.02f, 100, 1);
        glPopMatrix();
        glPopMatrix();
    }


    glColor3f(1.0f, 0.6f, 0.8f);
    renderText(-0.95f, 0.9f, "Patrick Lives: " + std::to_string(charLives));

    glColor3f(1.0f, 1.0f, 0.0f);
    renderText(-0.95f, 0.80f, "Spong Lives: " + std::to_string(SpongLives));

    glColor3f(1.0f, 0.0f, 0.0f);
    renderText(-0.95f, 0.70f, "mrKrabs Lives: " + std::to_string(mrKrabsLives));

    glutSwapBuffers();
}

void update(int value) {

    if (mrKrabsBulletActive) {
        mrKrabsBulletY += mrKrabsBulletSpeed;

        if (mrKrabsBulletX > SpongX - SpongWidth / 2 &&
            mrKrabsBulletX < SpongX + SpongWidth / 2 &&
            mrKrabsBulletY > SpongY - SpongHeight / 2 &&
            mrKrabsBulletY < SpongY + SpongHeight / 2) {
            SpongLives--;
            mrKrabsBulletActive = false;

            if (SpongLives <= 0) {
                gameOver = true;
                playerWon = false;
            }

        }
        if (mrKrabsBulletX > charX - charWidth / 2 &&
            mrKrabsBulletX < charX + charWidth / 2 &&
            mrKrabsBulletY > charY - charHeight / 2 &&
            mrKrabsBulletY < charY + charHeight / 2) {
            charLives--;
            mrKrabsBulletActive = false;

            if (charLives <= 0) {
                gameOver = true;
                playerWon = false;
            }
        }

        if (mrKrabsBulletY < -1.0f) {
            mrKrabsBulletActive = false;
        }
    }
    if (bulletActive) {
        bulletY += bulletSpeed;

        if (!mrKrabsHit &&
            bulletX > mrKrabsX - mrKrabsWidth / 2 &&
            bulletX < mrKrabsX + mrKrabsWidth / 2 &&
            bulletY > mrKrabsY - mrKrabsHeight / 2 &&
            bulletY < mrKrabsY + mrKrabsHeight / 2) {
            mrKrabsLives -= (powerUpShot ? 3 : 1);
            bulletActive = false;
            powerUpShot = false;

            if (mrKrabsLives <= 0) {
                gameOver = true;
                playerWon = true;
            }
        }

        if (bulletY > 1.0f) {
            bulletActive = false;
        }
    }

    if (SpongBulletActive) {
        SpongBulletY += SpongBulletSpeed;

        if (!mrKrabsHit &&
            SpongBulletX > mrKrabsX - mrKrabsWidth / 2 &&
            SpongBulletX < mrKrabsX + mrKrabsWidth / 2 &&
            SpongBulletY > mrKrabsY - mrKrabsHeight / 2 &&
            SpongBulletY < mrKrabsY + mrKrabsHeight / 2) {
            mrKrabsLives -= (SpongPowerShot ? 3 : 1);

            SpongBulletActive = false;

            if (mrKrabsLives <= 0) {
                gameOver = true;
                playerWon = true;
            }
        }

        if (SpongBulletY > 1.0f) {
            SpongBulletActive = false;
        }
    }
    if (SpongBulletActive) {
        SpongBulletY += SpongBulletSpeed;
        if (SpongBulletY > 1.0f) {
            SpongBulletActive = false;
            SpongPowerShot = false;
        }
    }

    if (rand() % 100 < 10) {
        moveMrKrabs();
    }
    if (powerUpActive && checkCollision(charX, charY, charWidth, charHeight, powerUpX, powerUpY, 0.04f, 0.04f)) {
        charLives++;
        PlaySound(TEXT("power ups.wav"), NULL, SND_FILENAME | SND_ASYNC);
        powerUpActive = false;
    }


    // Check for Spong's power-up collision
    if (powerUpActive && checkCollision(SpongX, SpongY, SpongWidth, SpongHeight, powerUpX, powerUpY, 0.04f, 0.04f)) {
        SpongLives++;
        PlaySound(TEXT("power ups.wav"), NULL, SND_FILENAME | SND_ASYNC);
        powerUpActive = false;
    }

    // Randomly spawn power-up
    if (rand() % 1000 < 5) { // Adjust probability as needed
        powerUpActive = true;
        powerUpX = (rand() % 100 - 50) / 50.0f; // Random x position
        powerUpY = (rand() % 100 - 50) / 50.0f; // Random y position
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void handleKeypress(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        if (charY < 0.9f) charY += 0.05f;
        break;
    case GLUT_KEY_DOWN:
        if (charY > -0.9f) charY -= 0.05f;
        break;
    case GLUT_KEY_LEFT:
        if (charX > -0.9f) charX -= 0.05f;
        break;
    case GLUT_KEY_RIGHT:
        if (charX < 0.9f) charX += 0.05f;
        break;
    }

    glutPostRedisplay();
}

void handleKeypressSpecial(unsigned char key, int x, int y) {
    if (key == 32 && !bulletActive) {
        bulletActive = true;
        bulletX = charX;
        bulletY = charY + charHeight / 2;
    }
    else if (key == 'x' || key == 'X') {
        powerUpShot = true;

        bulletActive = true;
        bulletX = charX;
        bulletY = charY + charHeight / 2;
    }

    if (key == 'w' || key == 'W') {
        if (SpongY < 0.9f) SpongY += 0.05f;
    }
    else if (key == 's' || key == 'S') {
        if (SpongY > -0.9f) SpongY -= 0.05f;
    }
    else if (key == 'a' || key == 'A') {
        if (SpongX > -0.9f) SpongX -= 0.05f;
    }
    else if (key == 'd' || key == 'D') {
        if (SpongX < 0.9f) SpongX += 0.05f;
    }

    if (key == 'e' || key == 'E') {
        if (!SpongBulletActive) {
            SpongBulletActive = true;
            SpongPowerShot = false;
            SpongBulletX = SpongX;
            SpongBulletY = SpongY + SpongHeight / 2;
        }
    }
    else if (key == 'f' || key == 'F') {
        if (!SpongBulletActive) {
            SpongBulletActive = true;
            SpongPowerShot = true;
            SpongBulletX = SpongX;
            SpongBulletY = SpongY + SpongHeight / 2;
        }
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    srand(static_cast<unsigned>(time(0)));
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Spong & Patrick");

    init();

    glutDisplayFunc(display);
    glutSpecialFunc(handleKeypress);
    glutKeyboardFunc(handleKeypressSpecial);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
