#pragma once

// Add this to objects that have colliders and want them to be solid,
// I'm debating about making this a bool in the BoxCollider, I don't see
// downsides to both
// Chose this way since in my opinion a boxcollider just checks for 
// collisions, it doesn't define what it does with the collision
// I could also just check what type of collider they have but if we have
// many diff solid colliders this can get redundant
struct SolidComponent
{

};
