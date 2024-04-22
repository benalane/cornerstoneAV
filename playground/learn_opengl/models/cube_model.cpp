float cubeVertices[] = {
    // Postition (3f), Color (3f)
    // 0: L B F
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    // 1: R B F
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    // 2: L T F
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    // 3: R T F
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
    // 4: L B N
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    // 5: R B N
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f,
    // 6: L T N
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
    // 7: R T N
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
};

unsigned int cubeElementIndexes[] {
    0, 1, 3, 0, 2, 3, // Far face
    0, 2, 6, 0, 4, 6, // Left face
    1, 3, 7, 1, 5, 7, // Right face
    2, 6, 7, 2, 3, 7, // Top face
    0, 1, 5, 0, 4, 5, // Bottom Face
    4, 5, 7, 4, 6, 7  // Near Face
};

float* getCubeVertices() {
    return cubeVertices;
}
unsigned int getCubeVerticesSize() {
    return sizeof(cubeVertices);
}
unsigned int* getCubeElementIndexes() {
    return cubeElementIndexes;
}
unsigned int getCubeElementIndexesSize(){
    return sizeof(cubeElementIndexes);
}