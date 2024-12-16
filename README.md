<!DOCTYPE html>
<html>
<body>
  <h1>Particle Simulation Using SFML</h1>

  <p>This project is a basic particle simulation built with C++ and <a href="https://www.sfml-dev.org/" target="_blank">SFML</a>. It demonstrates the application of gravity and user interaction to create and animate particles on the screen.</p>

  <h2>Features</h2>
  <ul>
    <li>Simulates particles with physics-based motion using Verlet integration.</li>
    <li>Applies gravity to all particles in real-time.</li>
    <li>Allows users to create particles by clicking the left mouse button.</li>
    <li>Dynamic rendering of multiple particles on the screen.</li>
  </ul>

  <h2>Requirements</h2>
  <ul>
    <li>C++17 or later</li>
    <li><a href="https://www.sfml-dev.org/download.php" target="_blank">SFML</a> (Simple and Fast Multimedia Library)</li>
    <li>A modern C++ compiler (e.g., g++, clang++)</li>
  </ul>

  <h2>How to Build and Run</h2>
  <ol>
    <li>Install SFML on your system by following the instructions on the <a href="https://www.sfml-dev.org/tutorials/" target="_blank">SFML tutorials</a>.</li>
    <li>Clone or download this repository.</li>
    <li>Navigate to the project directory and compile the code using the following command:</li>
    <pre><code>g++ physicsengine.cpp -o particle-sim -lsfml-graphics -lsfml-window -lsfml-system</code></pre>
    <li>Run the program:</li>
    <pre><code>./particle-sim</code></pre>
  </ol>

  <h2>How to Use</h2>
  <ul>
    <li>Click the left mouse button anywhere on the window to create a new particle at the cursor position.</li>
    <li>Watch the particles fall due to gravity.</li>
  </ul>

  <h2>Code Highlights</h2>
  <p>The core of the simulation uses Verlet integration to calculate particle motion:</p>
  <pre><code>void update(float dt) {
    sf::Vector2f new_pos = 2.f * position - position_last + acceleration * (dt * dt);
    position_last = position;
    position = new_pos;
    acceleration = sf::Vector2f(0, 0); // Reset acceleration for the next frame
}</code></pre>

  <h2>Future Enhancements</h2>
  <ul>
    <li>Add boundary collisions to restrict particles within the window.</li>
    <li>Implement particle lifespan and removal once out of bounds.</li>
    <li>Introduce visual effects like trails or fading colors.</li>
  </ul>

  <h2>Contributing</h2>
  <p>Feel free to fork this project, open issues, or submit pull requests to add features or fix bugs.</p>

  <h2>License</h2>
  <p>This project is open-source and available under the <a href="https://opensource.org/licenses/MIT" target="_blank">MIT License</a>.</p>

  <h2>Acknowledgments</h2>
  <ul>
    <li><a href="https://www.sfml-dev.org/" target="_blank">SFML</a> for its awesome library.</li>
    <li>All contributors to the C++ and game development communities for inspiration and resources.</li>
  </ul>
</body>
</html>
