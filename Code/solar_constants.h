#pragma once

struct OrbitalBody {
    float orbit_speed;
    float spin_speed;
    float orbit_radius;
    float scale;
    bool use_negative_orbit = false;
    float vertical_oscillation_amplitude = 0.0f;
    bool skip_spin = false;
};

namespace SolarSystem {

    // Orbital Speeds (dt multipliers)
    const float SUN_SPIN_SPEED = 0.5f;
    const float MERCURY_ORBIT_SPEED = 0.25f;
    const float MERCURY_SPIN_SPEED = 0.05f;
    const float VENUS_ORBIT_SPEED = 0.18f;
    const float VENUS_SPIN_SPEED = -0.01f;
    const float EARTH_ORBIT_SPEED = 0.2f;
    const float EARTH_SPIN_SPEED = 0.5f;
    const float MOON_ORBIT_SPEED = 2.0f;
    const float MOON_SPIN_SPEED = 3.0f;
    const float MARS_ORBIT_SPEED = 0.1f;
    const float MARS_SPIN_SPEED = 0.3f;
    const float PHOBOS_ORBIT_SPEED = 4.0f;
    const float PHOBOS_SPIN_SPEED = 1.0f;
    const float JUPITER_ORBIT_SPEED = 0.05f;
    const float JUPITER_SPIN_SPEED = 0.2f;
    const float IO_ORBIT_SPEED = 4.0f;
    const float IO_SPIN_SPEED = 1.0f;
    const float SATURN_ORBIT_SPEED = 0.02f;
    const float SATURN_SPIN_SPEED = 0.2f;
    const float TITAN_ORBIT_SPEED = 0.5f;
    const float TITAN_SPIN_SPEED = 0.05f;
    const float URANUS_ORBIT_SPEED = 0.01f;
    const float URANUS_SPIN_SPEED = -0.15f;
    const float NEPTUNE_ORBIT_SPEED = 0.005f;
    const float NEPTUNE_SPIN_SPEED = 0.1f;
    const float CERES_ORBIT_SPEED = 0.003f;
    const float HAUMEA_ORBIT_SPEED = 0.002f;
    const float ERIS_ORBIT_SPEED = 0.001f;

    // Orbital Radii (visual spacing)
    const float SUN_RADIUS = 0.0f;
    const float MERCURY_RADIUS = 2.0f;
    const float VENUS_RADIUS = 3.0f;
    const float EARTH_RADIUS = 4.0f;
    const float MOON_RADIUS = 2.0f;
    const float MARS_RADIUS = 6.0f;
    const float PHOBOS_RADIUS = 0.5f;
    const float JUPITER_RADIUS = 9.0f;
    const float IO_RADIUS = 1.5f;
    const float SATURN_RADIUS = 12.0f;
    const float TITAN_RADIUS = 1.8f;
    const float URANUS_RADIUS = 15.0f;
    const float NEPTUNE_RADIUS = 18.0f;
    const float CERES_RADIUS = 20.0f;
    const float HAUMEA_RADIUS = 23.0f;
    const float ERIS_RADIUS = 26.0f;

    // Scales (relative to Sun = 1.0)
    const float SUN_SCALE = 1.0f;
    const float MERCURY_SCALE = 0.15f;
    const float VENUS_SCALE = 0.35f;
    const float EARTH_SCALE = 0.5f;
    const float MOON_SCALE = 0.3f;
    const float MARS_SCALE = 0.4f;
    const float PHOBOS_SCALE = 0.1f;
    const float JUPITER_SCALE = 0.9f;
    const float IO_SCALE = 0.2f;
    const float SATURN_SCALE = 0.8f;
    const float TITAN_SCALE = 0.3f;
    const float URANUS_SCALE = 0.6f;
    const float NEPTUNE_SCALE = 0.55f;
    const float CERES_SCALE = 0.2f;
    const float HAUMEA_SCALE = 0.25f;
    const float ERIS_SCALE = 0.3f;

    // Orbital body definitions
    const OrbitalBody Sun = { 0.0f, SUN_SPIN_SPEED, SUN_RADIUS, SUN_SCALE };
    const OrbitalBody Mercury = { MERCURY_ORBIT_SPEED, MERCURY_SPIN_SPEED, MERCURY_RADIUS, MERCURY_SCALE };
    const OrbitalBody Venus = { VENUS_ORBIT_SPEED, VENUS_SPIN_SPEED, VENUS_RADIUS, VENUS_SCALE };
    const OrbitalBody Earth = { EARTH_ORBIT_SPEED, EARTH_SPIN_SPEED, EARTH_RADIUS, EARTH_SCALE };
    const OrbitalBody Moon = { MOON_ORBIT_SPEED, MOON_SPIN_SPEED, MOON_RADIUS, MOON_SCALE, true, 0.5f, true }; // Earth's Moon
    const OrbitalBody Mars = { MARS_ORBIT_SPEED, MARS_SPIN_SPEED, MARS_RADIUS, MARS_SCALE };
    const OrbitalBody Phobos = { PHOBOS_ORBIT_SPEED, PHOBOS_SPIN_SPEED, PHOBOS_RADIUS, PHOBOS_SCALE }; // Mars' Moon
    const OrbitalBody Jupiter = { JUPITER_ORBIT_SPEED, JUPITER_SPIN_SPEED, JUPITER_RADIUS, JUPITER_SCALE };
    const OrbitalBody Io = { IO_ORBIT_SPEED, IO_SPIN_SPEED, IO_RADIUS, IO_SCALE }; // Jupiter's Moon
    const OrbitalBody Saturn = { SATURN_ORBIT_SPEED, SATURN_SPIN_SPEED, SATURN_RADIUS, SATURN_SCALE };
    const OrbitalBody Titan = { TITAN_ORBIT_SPEED, TITAN_SPIN_SPEED, TITAN_RADIUS, TITAN_SCALE }; // Saturn's Moon
    const OrbitalBody Uranus = { URANUS_ORBIT_SPEED, URANUS_SPIN_SPEED, URANUS_RADIUS, URANUS_SCALE };
    const OrbitalBody Neptune = { NEPTUNE_ORBIT_SPEED, NEPTUNE_SPIN_SPEED, NEPTUNE_RADIUS, NEPTUNE_SCALE };
    const OrbitalBody Ceres = { CERES_ORBIT_SPEED, 0.05f, CERES_RADIUS, CERES_SCALE }; // Dwarf planet in asteroid belt
    const OrbitalBody Haumea = { HAUMEA_ORBIT_SPEED, 0.04f, HAUMEA_RADIUS, HAUMEA_SCALE }; // Dwarf planet in Kuiper belt
    const OrbitalBody Eris = { ERIS_ORBIT_SPEED, 0.03f, ERIS_RADIUS, ERIS_SCALE }; // Distant dwarf planet

} // namespace SolarSystem
