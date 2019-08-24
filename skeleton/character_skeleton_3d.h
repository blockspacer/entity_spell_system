#ifndef CHARACTER_SKELETON_H
#define CHARACTER_SKELETON_H

#ifdef ENTITIES_2D
#include "scene/2d/node_2d.h"
#else
#include "scene/3d/spatial.h"
#endif

#include "core/node_path.h"
#include "core/ustring.h"
#include "scene/animation/animation_player.h"
#include "scene/animation/animation_tree.h"

#include "../data/character_skeleton_visual_entry.h"
#include "../entity_enums.h"

#ifdef ENTITIES_2D

class CharacterSkeleton : public Node2D {
	GDCLASS(CharacterSkeleton, Node2D);

#else

class CharacterSkeleton : public Spatial {
	GDCLASS(CharacterSkeleton, Spatial);

#endif

public:
	NodePath get_bone_path(int index);
	void set_bone_path(int index, NodePath path);

	Ref<CharacterSkeletonVisualEntry> get_visual(int index);
	void set_visual(int index, Ref<CharacterSkeletonVisualEntry> entry);

	Node *get_bone_node(EntityEnums::CharacterSkeletonPoints node_id);

	NodePath get_animation_player_path();
	void set_animation_player_path(NodePath path);

	AnimationPlayer *get_animation_player();

	NodePath get_animation_tree_path();
	void set_animation_tree_path(NodePath path);

	AnimationTree *get_animation_tree();

	void update_nodes();

	CharacterSkeleton();

protected:
	static void _bind_methods();
	virtual void _notification(int p_notification);

private:
	NodePath _animation_player_path;
	NodePath _animation_tree_path;

    NodePath _bone_paths[EntityEnums::SKELETON_POINTS_MAX];
    
	AnimationPlayer *_animation_player;
	AnimationTree *_animation_tree;

	Node *_bone_nodes[EntityEnums::SKELETON_POINTS_MAX];

	Ref<CharacterSkeletonVisualEntry> _visuals[EntityEnums::SKELETON_POINTS_MAX];
};


#endif