#ifndef SPECIES_MODEL_DATA_H
#define SPECIES_MODEL_DATA_H

#include "core/resource.h"

#include "core/vector.h"
#include "core/ustring.h"
#include "core/color.h"
#include "scene/resources/packed_scene.h"

#include "../data/item_visual_entry.h"

#include "../entity_enums.h"

class ItemVisual;

class SpeciesModelData : public Resource {
	GDCLASS(SpeciesModelData, Resource);

public:
	int get_id();
	void set_id(int value);

	Ref<PackedScene> get_skeleton();
	void set_skeleton(Ref<PackedScene> value);

	//Entries
	Ref<ItemVisualEntry> get_visual(const int bone_index, const int index) const;
	void set_visual(const int bone_index, const int index, const Ref<ItemVisualEntry> visual);
	void add_visual(const int bone_index, const Ref<ItemVisualEntry> visual);
	void remove_visual(const int bone_index, const int index);

	int get_visual_count(const int bone_index) const;

	Vector<Variant> get_visuals(const int bone_index);
	void set_visuals(const int bone_index, const Vector<Variant> &visuals);

	//SkinColors
	Color get_skin_color(const int index) const;
	void set_skin_color(const int index, const Color skin_color);
	void add_skin_color(const Color skin_color);
	void remove_skin_color(const int index);

	int get_skin_color_count() const;

	Vector<Variant> get_skin_colors();
	void set_skin_colors(const Vector<Variant> &skin_colors);

	//HairStyles
	Ref<ItemVisualEntry> get_hair_style(const int index) const;
	void set_hair_style(const int index, const Ref<ItemVisualEntry> hair_style);
	void add_hair_style(const Ref<ItemVisualEntry> hair_style);
	void remove_hair_style(const int index);

	int get_hair_style_count() const;

	Vector<Variant> get_hair_styles();
	void set_hair_styles(const Vector<Variant> &hair_styles);

	//HairColors
	Color get_hair_color(const int index) const;
	void set_hair_color(const int index, const Color hair_color);
	void add_hair_color(const Color hair_color);
	void remove_hair_color(const int index);

	int get_hair_color_count() const;

	Vector<Variant> get_hair_colors();
	void set_hair_colors(const Vector<Variant> &hair_colors);

	//Heads
	Ref<ItemVisualEntry> get_head(const int index) const;
	void set_head(const int index, const Ref<ItemVisualEntry> head);
	void add_head(const Ref<ItemVisualEntry> head);
	void remove_head(const int index);

	int get_head_count() const;

	Vector<Variant> get_heads();
	void set_heads(const Vector<Variant> &heads);

	SpeciesModelData();
	~SpeciesModelData();

protected:
	static void _bind_methods();

private:
	int _id;

	Ref<PackedScene> _skeleton;

	Vector<Ref<ItemVisualEntry> > _visuals[EntityEnums::SKELETON_POINTS_MAX];
	Vector<Color> _skin_colors;
	Vector<Ref<ItemVisualEntry> > _hair_styles;
	Vector<Color> _hair_colors;
	Vector<Ref<ItemVisualEntry> > _heads;
	
};


#endif
