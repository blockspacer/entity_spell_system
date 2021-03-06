#include "entity.h"

#include "../data/aura.h"
#include "../data/spell.h"
#include "../entities/auras/aura_data.h"
#include "../infos/spell_cast_info.h"
#include "../inventory/bag.h"
#include "../pipelines/spell_damage_info.h"
#include "../pipelines/spell_heal_info.h"
#include "./data/character_spec.h"
#include "./data/talent_row_data.h"
#include "./skills/entity_skill.h"

NodePath Entity::get_body_path() {
	return _body_path;
}
void Entity::set_body_path(NodePath value) {
	_body_path = value;

	_body = get_node_or_null(_body_path);

	if (ObjectDB::instance_validate(_body))
		_body->set_owner(this);
}
Node *Entity::get_body() {
	return _body;
}

NodePath Entity::get_character_skeleton_path() {
	return _character_skeleton_path;
}
void Entity::set_character_skeleton_path(NodePath value) {
	_character_skeleton_path = value;

	_character_skeleton = get_node_or_null(_character_skeleton_path);
}
Node *Entity::get_character_skeleton() {
	return _character_skeleton;
}

EntityEnums::EntityType Entity::gets_entity_type() {
	return _s_entity_type;
}
void Entity::sets_entity_type(EntityEnums::EntityType value) {
	_s_entity_type = value;

	VRPC(setc_entity_type, value);
}

EntityEnums::EntityType Entity::getc_entity_type() {
	return _c_entity_type;
}
void Entity::setc_entity_type(EntityEnums::EntityType value) {
	_c_entity_type = value;
}

//EntityInteractionType
EntityEnums::EntityInteractionType Entity::gets_entity_interaction_type() {
	return _s_interaction_type;
}
void Entity::sets_entity_interaction_type(EntityEnums::EntityInteractionType value) {
	_s_interaction_type = value;

	VRPC(setc_entity_interaction_type, value);
}

EntityEnums::EntityInteractionType Entity::getc_entity_interaction_type() {
	return _c_interaction_type;
}
void Entity::setc_entity_interaction_type(EntityEnums::EntityInteractionType value) {
	_c_interaction_type = value;
}

int Entity::gets_immunity_flags() {
	return _s_immunity_flags;
}
void Entity::sets_immunity_flags(int value) {
	_s_immunity_flags = value;
}

int Entity::gets_entity_flags() {
	return _s_entity_flags;
}
void Entity::sets_entity_flags(int value) {
	_s_entity_flags = value;

	VRPC(setc_entity_flags, value);
}

int Entity::getc_entity_flags() {
	return _c_entity_flags;
}
void Entity::setc_entity_flags(int value) {
	_c_entity_flags = value;
}

String Entity::gets_entity_name() {
	return _s_entity_name;
}
void Entity::sets_entity_name(String value) {
	_s_entity_name = value;

	emit_signal("sname_changed", this);

	VRPC(setc_entity_name, value);
}

String Entity::getc_entity_name() {
	return _c_entity_name;
}
void Entity::setc_entity_name(String value) {
	_c_entity_name = value;

	emit_signal("cname_changed", this);
}

EntityEnums::EntityGender Entity::gets_gender() {
	return _s_gender;
}
void Entity::sets_gender(EntityEnums::EntityGender value) {
	_s_gender = value;

	VRPC(setc_gender, value);
}

EntityEnums::EntityGender Entity::getc_gender() {
	return _c_gender;
}
void Entity::setc_gender(EntityEnums::EntityGender value) {
	_c_gender = value;

	if (ObjectDB::instance_validate(_character_skeleton)) {
		if (_character_skeleton->has_method("set_gender"))
			_character_skeleton->call("set_gender", _c_gender);
	}
}

int Entity::gets_level() {
	return _s_level;
}
void Entity::sets_level(int value) {
	_s_level = value;

	emit_signal("son_level_changed", this, value);

	VRPC(setc_level, value);
}

int Entity::getc_level() {
	return _c_level;
}
void Entity::setc_level(int value) {
	_c_level = value;

	emit_signal("con_level_changed", this, value);
}

int Entity::gets_xp() {
	return _s_xp;
}
void Entity::sets_xp(int value) {
	_s_xp = value;

	ORPC(setc_xp, value);
}

int Entity::getc_xp() {
	return _c_xp;
}
void Entity::setc_xp(int value) {
	_c_xp = value;
}

int Entity::gets_money() {
	return _s_money;
}
void Entity::sets_money(int value) {
	_s_money = value;

	ORPC(setc_money, value);
}

int Entity::getc_money() {
	return _c_money;
}
void Entity::setc_money(int value) {
	_c_money = value;
}

int Entity::gets_entity_data_id() {
	return _s_class_id;
}

void Entity::sets_entity_data_id(int value) {
	_s_class_id = value;
}

int Entity::getc_entity_data_id() {
	return _c_class_id;
}

void Entity::setc_entity_data_id(int value) {
	_c_class_id = value;

	if (_c_class_id == 0) {
		setc_entity_data(Ref<EntityData>());
		return;
	}

	if (EntityDataManager::get_instance() != NULL) {
		setc_entity_data(EntityDataManager::get_instance()->get_entity_data(_c_class_id));
	}
}

Ref<EntityData> Entity::gets_entity_data() {
	return _s_entity_data;
}

void Entity::sets_entity_data(Ref<EntityData> value) {
	_s_class_id = 0;

	if (value.is_valid()) {
		_s_class_id = value->get_id();
	}

	_s_entity_data = value;

	setup();

	emit_signal("sentity_data_changed", value);

	VRPC(setc_entity_data_id, _s_class_id);
}

Ref<EntityData> Entity::getc_entity_data() {
	return _c_entity_data;
}

void Entity::setc_entity_data(Ref<EntityData> value) {
	_c_entity_data = value;

	emit_signal("centity_data_changed", value);
}

EntityEnums::AIStates Entity::gets_ai_state() const {
	return _sai_state;
}
void Entity::sets_ai_state(EntityEnums::AIStates state) {
	_sai_state = state;
}

EntityEnums::AIStates Entity::gets_ai_state_stored() const {
	return _sai_state_stored;
}
void Entity::sets_ai_state_stored(EntityEnums::AIStates state) {
	_sai_state_stored = state;
}

int Entity::gets_seed() {
	return _s_seed;
}
void Entity::sets_seed(int value) {
	_s_seed = value;

	ORPC(setc_seed, value);
}

int Entity::getc_seed() {
	return _c_seed;
}
void Entity::setc_seed(int value) {
	_c_seed = value;
}

void Entity::setup() {
	if (has_method("_setup")) {
		call_multilevel("_setup");
	}
}

void Entity::_setup() {
	if (!_s_entity_data.is_valid())
		return;

	if (_deserialized) {
		Ref<EntityClassData> cc = gets_entity_data()->get_entity_class_data();
		ERR_FAIL_COND(!cc.is_valid());

		Ref<StatData> stat_data = cc->get_stat_data();

		ERR_FAIL_COND(!stat_data.is_valid());

		for (int i = 0; i < Stat::STAT_ID_TOTAL_STATS; ++i) {
			Ref<StatDataEntry> sde = stat_data->get_stat_data_int(i);

			_stats[i]->set_stat_data_entry(sde);
		}

		sets_ai(_s_entity_data->get_ai_instance());

		for (int i = 0; i < _s_auras.size(); ++i) {
			Ref<AuraData> ad = _s_auras.get(i);

			if (!ad->get_aura()->get_hide())
				VRPCOBJ(cadd_aura_rpc, JSON::print(ad->to_dict()), cadd_aura, ad);
		}

		for (int i = 0; i < _s_resources.size(); ++i) {
			Ref<EntityResource> res = _s_resources.get(i);

			ERR_CONTINUE(!res.is_valid());

			res->resolve_references();

			//SEND
		}

		return;
	}

	ERR_FAIL_COND(!gets_entity_data().is_valid());

	Ref<EntityClassData> cc = gets_entity_data()->get_entity_class_data();

	ERR_FAIL_COND(!cc.is_valid());

	for (int i = 0; i < Stat::STAT_ID_TOTAL_STATS; ++i) {
		cc->get_stat_data()->get_stat_for_stat(_stats[i]);
	}

	for (int i = 0; i < Stat::STAT_ID_TOTAL_STATS; ++i) {
		Ref<Stat> s = _stats[i];

		s->apply_modifiers();

		s->setc_values(s->gets_current(), s->gets_max());
		s->set_dirty(false);
	}

	for (int i = 0; i < cc->get_num_auras(); ++i) {
		Ref<Aura> a = cc->get_aura(i);

		if (a.is_valid()) {
			a->sapply_simple(this, this, 1.0);
		}
	}

	_s_entity_data->setup_resources(this);

	sets_entity_data_id(_s_entity_data->get_id());

	sets_entity_type(_s_entity_data->get_entity_type());
	sets_entity_interaction_type(_s_entity_data->get_entity_interaction_type());
	sets_immunity_flags(_s_entity_data->get_immunity_flags());
	sets_entity_flags(_s_entity_data->get_entity_flags());

	if (_s_entity_controller == EntityEnums::ENITIY_CONTROLLER_NONE) {
		sets_original_entity_controller(_s_entity_data->get_entity_controller());
		sets_entity_controller(_s_entity_data->get_entity_controller());
	}

	//sets_entity_name(_s_entity_data->get_entity_name());
	sets_money(_s_entity_data->get_money());

	Ref<EntityClassData> cd = _s_entity_data->get_entity_class_data();

	if (cd.is_valid()) {
		for (int i = 0; i < cd->get_num_start_spells(); ++i) {
			adds_spell(cd->get_start_spell(i));
		}
	}

	for (int i = 0; i < _s_entity_data->get_num_craft_recipes(); ++i) {
		adds_craft_recipe(_s_entity_data->get_craft_recipe(i));
	}

	if (_s_entity_data->get_equipment_data().is_valid()) {
		Ref<EquipmentData> eqd = _s_entity_data->get_equipment_data();

		for (int i = 0; i < ItemEnums::EQUIP_SLOT_EQUIP_SLOT_MAX; ++i) {
			Ref<ItemInstance> ii = eqd->get_item(i);

			if (ii.is_valid())
				_s_equipment[i] = ii;
		}
	}

	for (int i = 0; i < _s_resources.size(); ++i) {
		Ref<EntityResource> res = _s_resources.get(i);

		ERR_CONTINUE(!res.is_valid());

		res->resolve_references();
	}

	sets_ai(_s_entity_data->get_ai_instance());

	if (!Engine::get_singleton()->is_editor_hint())
		set_process(_s_entity_data.is_valid());
}

void Entity::setup_actionbars() {
	if (!gets_entity_data().is_valid())
		return;

	if (is_deserialized()) {

		return;
	}

	ProfileManager *pm = ProfileManager::get_instance();

	if (pm != NULL) {
		Ref<ClassProfile> cp = pm->get_class_profile(gets_entity_data()->get_id());

		if (cp.is_valid()) {
			set_actionbar_locked(cp->get_actionbar_locked());

			get_action_bar_profile()->clear_action_bars();

			Ref<ActionBarProfile> abp = cp->get_action_bar_profile();

			get_action_bar_profile()->from_actionbar_profile(abp);
		}
	}

	if (!gets_bag().is_valid()) {

		Ref<Bag> bag;
		bag.instance();

		bag->set_size(gets_entity_data()->get_bag_size());

		sets_bag(bag);
	}
}

// AI

bool Entity::gets_is_pet() {
	return _s_pet_owner;
}

bool Entity::getc_is_pet() {
	return _c_pet_owner;
}

Entity *Entity::gets_pet_owner() {
	return _s_pet_owner;
}
void Entity::sets_pet_owner(Entity *entity) {
	_s_pet_owner = entity;
}
void Entity::sets_pet_owner_bind(Node *entity) {
	if (!entity) {
		return;
	}

	Entity *e = cast_to<Entity>(entity);

	if (!e) {
		return;
	}

	return sets_pet_owner(e);
}

int Entity::gets_pet_formation_index() {
	return _s_pet_formation_index;
}
void Entity::sets_pet_formation_index(int value) {
	_s_pet_formation_index = value;
}

EntityEnums::AIStates Entity::gets_pet_ai_state() {
	return _s_pet_ai_state;
}
void Entity::sets_pet_ai_state(EntityEnums::AIStates value) {
	_s_pet_ai_state = value;
}

EntityEnums::EntityController Entity::gets_original_entity_controller() {
	return _s_entity_controller;
}
void Entity::sets_original_entity_controller(EntityEnums::EntityController value) {
	_s_entity_controller = value;
}

EntityEnums::EntityController Entity::gets_entity_controller() {
	return _s_entity_controller;
}
void Entity::sets_entity_controller(EntityEnums::EntityController value) {
	_s_entity_controller = value;
}

Ref<EntityAI> Entity::gets_ai() {
	return _s_ai;
}
void Entity::sets_ai(Ref<EntityAI> value) {
	if (_s_ai.is_valid()) {
		_s_ai->set_owner(NULL);
		_s_ai.unref();
	}

	_s_ai = value;
	_s_ai->set_owner(this);
}


////    Pets    ////

void Entity::adds_pet(Entity *entity) {
	ERR_FAIL_COND(!ObjectDB::instance_validate(entity));

	//the owner always want to see his pet, and you pet will always want to see the owner
	adds_sees(entity);
	entity->adds_sees(this);

	entity->sets_pet_owner(this);

	_s_pets.push_back(entity);

	entity->sets_ai_state_stored(entity->gets_ai_state());
	entity->sets_ai_state(_s_pet_ai_state);
	entity->sets_entity_controller(EntityEnums::ENITIY_CONTROLLER_AI);

	entity->sets_pet_formation_index(_s_pets.size());

	//full callback stack spet_added
}
void Entity::adds_pet_bind(Node *entity) {
	Entity *e = Object::cast_to<Entity>(entity);

	ERR_FAIL_COND(!e);

	adds_pet(e);
}
Entity *Entity::gets_pet(int index) {
	ERR_FAIL_INDEX_V(index, _s_pets.size(), NULL);

	return _s_pets.get(index);
}
void Entity::removes_pet_index(int index) {
	ERR_FAIL_INDEX(index, _s_pets.size());

	Entity *entity = _s_pets.get(index);

	_s_pets.remove(index);

	removes_sees(entity);

	for (int i = 0; i < _s_pets.size(); ++i) {
		Entity *pet = _s_pets.get(index);

		ERR_CONTINUE(!ObjectDB::instance_validate(entity));
		
		_s_pets.get(i)->sets_pet_formation_index(i);
	}

	ERR_FAIL_COND(!ObjectDB::instance_validate(entity));

	entity->sets_pet_owner(NULL);

	entity->sets_ai_state(entity->gets_ai_state_stored());
	entity->sets_entity_controller(entity->gets_original_entity_controller());

	//full callback stack spet_added
}
void Entity::removes_pet(Entity *entity) {
	for (int i = 0; i < _s_pets.size(); ++i) {
		if (_s_pets.get(i) == entity) {
			removes_pet_index(i);
			return;
		}
	}
}
void Entity::removes_pet_bind(Node *entity) {
	Entity *e = Object::cast_to<Entity>(entity);

	ERR_FAIL_COND(!e);

	removes_pet(e);
}
int Entity::gets_pet_count() {
	return _s_pets.size();
}

void Entity::addc_pet_path(NodePath path) {
	Node *n = get_node_or_null(path);

	Entity *entity = Object::cast_to<Entity>(n);

	ERR_FAIL_COND(!ObjectDB::instance_validate(entity));

	addc_pet(entity);
}

void Entity::addc_pet(Entity *entity) {
	ERR_FAIL_COND(!ObjectDB::instance_validate(entity));

	_c_pets.push_back(entity);

	//full callback stack spet_added
}
void Entity::addc_pet_bind(Node *entity) {
	Entity *e = Object::cast_to<Entity>(entity);

	ERR_FAIL_COND(!e);

	addc_pet(e);
}
Entity *Entity::getc_pet(int index) {
	ERR_FAIL_INDEX_V(index, _c_pets.size(), NULL);

	return _c_pets.get(index);
}
void Entity::removec_pet_index(int index) {
	ERR_FAIL_INDEX(index, _c_pets.size());

	Entity *entity = _c_pets.get(index);

	_c_pets.remove(index);

	ERR_FAIL_COND(!ObjectDB::instance_validate(entity));

	//full callback stack spet_added
}
void Entity::removec_pet(Entity *entity) {
	for (int i = 0; i < _c_pets.size(); ++i) {
		if (_c_pets.get(i) == entity) {
			removec_pet_index(i);
			return;
		}
	}
}
void Entity::removec_pet_bind(Node *entity) {
	Entity *e = Object::cast_to<Entity>(entity);

	ERR_FAIL_COND(!e);

	removec_pet(e);
}
int Entity::getc_pet_count() {
	return _s_pets.size();
}

////    Serialization    ////

bool Entity::is_deserialized() {
	return _deserialized;
}

Dictionary Entity::to_dict() {
	return call("_to_dict");
}
void Entity::from_dict(const Dictionary &dict) {
	_deserialized = true;

	call("_from_dict", dict);

	emit_signal("deserialized", this);
}

Dictionary Entity::_to_dict() {
	Dictionary dict;

	////    PlayerData    ////

	dict["guid"] = _s_guid;
	//dict["entity_data_id"] = _s_class_id;
	dict["type"] = _s_type;
	dict["gender"] = _s_gender;
	dict["level"] = _s_level;
	dict["xp"] = _s_xp;
	dict["money"] = _s_money;
	dict["seed"] = _s_seed;

	if (_s_entity_data.is_valid())
		dict["entity_data_id"] = _s_entity_data->get_id();
	else
		dict["entity_data_id"] = 0;

	//dict["send_flag"] = _s_send_flag;
	dict["entity_name"] = _s_entity_name;

	////     Stats    ////

	Dictionary sd;

	for (int i = 0; i < Stat::STAT_ID_TOTAL_STATS; ++i) {
		Ref<Stat> s = _stats[i];

		sd[i] = s->to_dict();
	}

	dict["stats"] = sd;

	////    Equipment    ////

	Dictionary equipment;

	for (int i = 0; i < ItemEnums::EQUIP_SLOT_EQUIP_SLOT_MAX; ++i) {
		Ref<ItemInstance> ii = _s_equipment[i];

		if (ii.is_valid())
			equipment[i] = ii->to_dict();
	}

	dict["equipment"] = equipment;

	////    Resources    ////

	Dictionary rd;

	for (int i = 0; i < _s_resources.size(); ++i) {
		rd[i] = _s_resources.get(i)->to_dict();
	}

	dict["resources"] = rd;

	////    GCD    ////

	dict["gcd"] = _s_gcd;

	////    States    ////

	Dictionary stated;

	for (int i = 0; i < EntityEnums::ENTITY_STATE_TYPE_INDEX_MAX; ++i) {
		stated[i] = _s_states[i];
	}

	dict["states"] = stated;

	dict["state"] = _s_state;

	////    SpellCastData    ////

	//Not needed
	//Ref<SpellCastInfo> _s_spell_cast_info;
	//Ref<SpellCastInfo> _c_spell_cast_info;

	//// AuraComponent    ////

	Dictionary auras;

	for (int i = 0; i < _s_auras.size(); ++i) {
		auras[i] = _s_auras.get(i)->to_dict();
	}

	dict["auras"] = auras;

	dict["entity_type"] = _s_entity_type;
	dict["immunity_flags"] = _s_immunity_flags;
	dict["entity_flags"] = _s_entity_flags;
	dict["entity_controller"] = _s_entity_controller;

	////    Cooldowns    ////

	Dictionary cds;

	for (int i = 0; i < _s_cooldowns.size(); ++i) {
		cds[i] = _s_cooldowns.get(i)->to_dict();
	}

	dict["cooldowns"] = cds;

	Dictionary ccds;

	for (int i = 0; i < _s_category_cooldowns.size(); ++i) {
		ccds[i] = _s_category_cooldowns.get(i)->to_dict();
	}

	dict["category_cooldowns"] = ccds;

	dict["active_category_cooldowns"] = _s_active_category_cooldowns;

	////    Talents    ////

	dict["free_talent_points"] = _s_free_talent_points;
	dict["talents"] = _s_talents;

	////    Data    ////

	Array entity_datas;

	for (int i = 0; i < _s_data.size(); ++i) {
		entity_datas.append(_s_data.get(i)->to_dict());
	}

	dict["entity_datas"] = entity_datas;

	////    Crafting    ////

	Dictionary known_recipes;

	for (int i = 0; i < _s_craft_recipes.size(); ++i) {
		known_recipes[i] = _s_craft_recipes.get(i)->get_id();
	}

	dict["known_recipes"] = known_recipes;

	////    Known Spells    ////

	dict["free_spell_points"] = _s_free_spell_points;

	Dictionary known_spells;

	for (int i = 0; i < _s_spells.size(); ++i) {
		known_spells[i] = _s_spells.get(i)->get_id();
	}

	dict["known_spells"] = known_spells;

	////    Skills    ////

	Dictionary skills;

	for (int i = 0; i < _s_skills.size(); ++i) {
		skills[i] = _s_skills.get(i)->to_dict();
	}

	dict["skills"] = skills;

	////    Bags    ////

	if (_s_bag.is_valid())
		dict["bag"] = _s_bag->to_dict();

	////     Actionbars    ////

	dict["actionbar_locked"] = _actionbar_locked;
	dict["actionbar_profile"] = _action_bar_profile->to_dict();

	return dict;
}
void Entity::_from_dict(const Dictionary &dict) {
	ERR_FAIL_COND(dict.empty());

	sets_entity_type((EntityEnums::EntityType)((int)dict.get("type", 0)));

	sets_gender(static_cast<EntityEnums::EntityGender>(static_cast<int>(dict.get("gender", 0))));
	sets_level(dict.get("level", 0));
	sets_xp(dict.get("xp", 0));
	sets_money(dict.get("money", 0));

	sets_entity_name(dict.get("entity_name", ""));

	sets_seed(dict.get("seed", _s_seed));

	////     Stats    ////

	Dictionary stats = dict.get("stats", Dictionary());

	for (int i = 0; i < Stat::STAT_ID_TOTAL_STATS; ++i) {
		Ref<Stat> s = _stats[i];

		s->from_dict(stats.get(String::num(i), Dictionary()));
	}

	////    Equipment    ////

	Dictionary equipment = dict.get("equipment", Dictionary());

	for (int i = 0; i < ItemEnums::EQUIP_SLOT_EQUIP_SLOT_MAX; ++i) {
		if (equipment.has(String::num(i))) {
			Ref<ItemInstance> ii = _s_equipment[i];

			if (!ii.is_valid()) {
				ii.instance();
			}

			ii->from_dict(dict[String::num(i)]);

			_s_equipment[i] = ii;
			_c_equipment[i] = ii;
		}
	}

	////    Resources    ////

	_s_resources.clear();
	_c_resources.clear();

	Dictionary rd = dict.get("resources", Dictionary());

	for (int i = 0; i < rd.size(); ++i) {
		Ref<EntityResource> r;
		r.instance();

		r->from_dict(rd.get(String::num(i), Dictionary()));

		adds_resource(r);
	}

	////    GCD    ////

	_s_gcd = dict.get("gcd", 0);
	_c_gcd = _s_gcd;

	////    States    ////

	Dictionary statesd = dict.get("states", Dictionary());

	for (int i = 0; i < EntityEnums::ENTITY_STATE_TYPE_INDEX_MAX; ++i) {
		_s_states[i] = statesd.get(String::num(i), 0);
	}

	_s_state = dict.get("state", Dictionary());
	_c_state = _s_state;

	////    Auras    ////

	_s_auras.clear();
	_c_auras.clear();

	Dictionary auras = dict.get("auras", Dictionary());

	for (int i = 0; i < auras.size(); ++i) {
		Ref<AuraData> r;
		r.instance();

		r->from_dict(auras.get(String::num(i), Dictionary()));
		r->set_owner(this);
		r->resolve_references(this);

		_s_auras.push_back(r);
		//_c_auras.push_back(r);
	}

	sets_entity_type((EntityEnums::EntityType)((int)dict.get("entity_type", 0)));
	sets_immunity_flags(dict.get("immunity_flags", 0));
	sets_entity_flags(dict.get("entity_flags", 0));
	EntityEnums::EntityController contr = static_cast<EntityEnums::EntityController>(static_cast<int>(dict.get("entity_controller", 0)));

	sets_original_entity_controller(contr);
	sets_entity_controller(contr);

	////    Cooldowns    ////

	_s_cooldowns.clear();
	_c_cooldowns.clear();

	Dictionary cds = dict.get("cooldowns", Dictionary());

	for (int i = 0; i < cds.size(); ++i) {
		Ref<Cooldown> cd;
		cd.instance();

		cd->from_dict(cds.get(String::num(i), Dictionary()));

		_s_cooldowns.push_back(cd);
		_c_cooldowns.push_back(cd);
	}

	Dictionary ccds = dict.get("category_cooldowns", Dictionary());

	for (int i = 0; i < ccds.size(); ++i) {
		Ref<CategoryCooldown> ccd;
		ccd.instance();

		ccd->from_dict(ccds.get(String::num(i), Dictionary()));

		_s_category_cooldowns.push_back(ccd);
		_c_category_cooldowns.push_back(ccd);
	}

	_s_active_category_cooldowns = dict.get("active_category_cooldowns", 0);
	_c_active_category_cooldowns = _s_active_category_cooldowns;

	////    Talents    ////

	_s_free_talent_points = dict.get("free_talent_points", 0);
	_c_free_talent_points = _s_free_talent_points;

	Vector<int> talents = dict.get("talents", Vector<int>());

	for (int i = 0; i < talents.size(); ++i) {
		adds_talent(talents[i]);
	}

	////    Data    ////

	Array entity_datas = dict.get("entity_datas", Array());

	for (int i = 0; i < entity_datas.size(); ++i) {
		Dictionary entry = entity_datas.get(i);

		String class_name = dict.get("class_name", EntityDataContainer::get_class_static());

		if (ClassDB::can_instance(class_name) && ClassDB::is_parent_class(class_name, EntityDataContainer::get_class_static())) {
			Ref<EntityDataContainer> data = Ref<EntityDataContainer>(ClassDB::instance(class_name));

			if (data.is_valid()) {
				data->from_dict(entry);

				_s_data.push_back(data);
				_c_data.push_back(data);
			}
		}
	}

	////    Crafting    ////

	_s_craft_recipes.clear();
	_c_craft_recipes.clear();

	Dictionary known_recipes = dict.get("known_recipes", Dictionary());

	for (int i = 0; i < known_recipes.size(); ++i) {
		int crid = known_recipes.get(String::num(i), 0);

		if (EntityDataManager::get_instance() != NULL) {
			Ref<CraftRecipe> cr = EntityDataManager::get_instance()->get_craft_data(crid);

			if (cr.is_valid()) {
				adds_craft_recipe(cr);
			}
		}
	}

	////    Known Spells    ////

	sets_free_spell_points(dict.get("free_spell_points", 0));

	Dictionary known_spells = dict.get("known_spells", Dictionary());

	for (int i = 0; i < known_spells.size(); ++i) {
		int spell_id = known_spells.get(String::num(i), 0);

		if (EntityDataManager::get_instance() != NULL) {
			Ref<Spell> sp = EntityDataManager::get_instance()->get_spell(spell_id);

			if (sp.is_valid()) {
				_s_spells.push_back(sp);
				_c_spells.push_back(sp);
			}
		}
	}

	////    Skills    ////

	Dictionary skills = dict.get("skills", Dictionary());

	for (int i = 0; i < skills.size(); ++i) {
		Ref<EntitySkill> r;
		r.instance();

		r->from_dict(skills.get(String::num(i), Dictionary()));

		_s_skills.push_back(r);
		_c_skills.push_back(r);
	}

	////    Bags    ////

	Dictionary bagd = dict.get("bag", Dictionary());

	if (!bagd.empty()) {
		if (!_s_bag.is_valid()) {
			Ref<Bag> bag;
			bag.instance();

			bag->from_dict(bagd);

			sets_bag(bag);
		} else {
			_s_bag->from_dict(bagd);
		}
	}

	////     Actionbars    ////

	_actionbar_locked = dict.get("actionbar_locked", false);
	_action_bar_profile->from_dict(dict.get("actionbar_profile", Dictionary()));

	int edi = dict.get("entity_data_id", 0);

	if (EntityDataManager::get_instance() != NULL) {
		sets_entity_data(EntityDataManager::get_instance()->get_entity_data(edi));
	}

	sets_entity_data_id(edi);
}

void Entity::initialize(Ref<EntityCreateInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	_s_entity_name = info->get_entity_name();
	_c_entity_name = info->get_entity_name();

	sets_original_entity_controller(info->get_entity_controller());
	sets_entity_controller(info->get_entity_controller());
	//setc_entity_controller(info->get_entity_type());

	sets_level(info->get_level());
	setc_level(info->get_level());

	sets_xp(info->get_xp());
	setc_xp(info->get_xp());

	sets_entity_data(info->get_entity_data());
}

//////     Stat System      //////

bool Entity::gets_is_dead() {
	return _s_is_dead;
}

bool Entity::getc_is_dead() {
	return _c_is_dead;
}

bool Entity::getc_has_global_cooldown() {
	return _c_gcd >= 0.000000001;
}

bool Entity::gets_has_global_cooldown() {
	return _s_gcd >= 0.000000001;
}

bool Entity::getc_global_cooldown() {
	return _c_gcd;
}

bool Entity::gets_global_cooldown() {
	return _s_gcd;
}

void Entity::sstart_global_cooldown(float value) {
	_s_gcd = value;

	void son_gcd_started();

	emit_signal("sgcd_started", _s_gcd);

	ORPC(cstart_global_cooldown, value);
}

void Entity::cstart_global_cooldown(float value) {
	_c_gcd = value;

	void con_gcd_started();

	emit_signal("cgcd_started", _c_gcd);
}

////    States    ////

int Entity::gets_state() {
	return _s_state;
}
void Entity::sets_state(int state) {
	_s_state = state;

	emit_signal("sstate_changed", state);

	VRPC(setc_state, state);
}

int Entity::getc_state() {
	return _c_state;
}
void Entity::setc_state(int state) {
	_c_state = state;

	emit_signal("cstate_changed", state);
}

void Entity::sadd_state_ref(int state_index) {
	ERR_FAIL_INDEX(state_index, EntityEnums::ENTITY_STATE_TYPE_INDEX_MAX);

	if (_s_states[state_index]++ == 0) {
		sets_state(gets_state() | EntityEnums::get_state_flag_for_index(state_index));
	}
}

void Entity::sremove_state_ref(int state_index) {
	ERR_FAIL_INDEX(state_index, EntityEnums::ENTITY_STATE_TYPE_INDEX_MAX);

	if (--_s_states[state_index] == 0) {
		sets_state(gets_state() ^ EntityEnums::get_state_flag_for_index(state_index));
	}
}

////    Crafting System    ////

void Entity::crequest_craft(int id) {
	scraft(id);
}
void Entity::scraft(int id) {
	if (has_method("_scraft")) {
		call("_scraft", id);
	}
}

bool Entity::hass_craft_recipe(Ref<CraftRecipe> craft_recipe) {
	for (int i = 0; i < _s_craft_recipes.size(); ++i) {
		if (_s_craft_recipes.get(i) == craft_recipe) {
			return true;
		}
	}

	return false;
}
bool Entity::hass_craft_recipe_id(int id) {
	for (int i = 0; i < _s_craft_recipes.size(); ++i) {
		Ref<CraftRecipe> cr = _s_craft_recipes.get(i);

		ERR_CONTINUE(!cr.is_valid());

		if (cr->get_id() == id) {
			return true;
		}
	}

	return false;
}
void Entity::adds_craft_recipe(Ref<CraftRecipe> craft_recipe) {
	ERR_FAIL_COND(!craft_recipe.is_valid());

	if (hass_craft_recipe(craft_recipe))
		return;

	_s_craft_recipes.push_back(craft_recipe);

	emit_signal("scraft_recipe_added", this, craft_recipe);

	ORPC(addc_craft_recipe_id, craft_recipe->get_id());
}
void Entity::adds_craft_recipe_id(int id) {
	ERR_FAIL_COND(!EntityDataManager::get_instance());

	if (hass_craft_recipe_id(id))
		return;

	Ref<CraftRecipe> craft_recipe = EntityDataManager::get_instance()->get_craft_data(id);

	ERR_FAIL_COND(!craft_recipe.is_valid());

	_s_craft_recipes.push_back(craft_recipe);

	emit_signal("scraft_recipe_added", this, craft_recipe);

	ORPC(addc_craft_recipe_id, id);
}
void Entity::removes_craft_recipe(Ref<CraftRecipe> craft_recipe) {
	for (int i = 0; i < _s_craft_recipes.size(); ++i) {
		if (_s_craft_recipes.get(i) == craft_recipe) {
			_s_craft_recipes.remove(i);
			break;
		}
	}

	emit_signal("scraft_recipe_removed", this, craft_recipe);

	ORPC(removec_craft_recipe, craft_recipe);
}
void Entity::removes_craft_recipe_id(int id) {
	Ref<CraftRecipe> craft_recipe;

	for (int i = 0; i < _s_craft_recipes.size(); ++i) {
		craft_recipe = _s_craft_recipes.get(i);

		if (craft_recipe->get_id() == id) {
			_s_craft_recipes.remove(i);
			break;
		}
	}

	emit_signal("scraft_recipe_removed", this, craft_recipe);

	ORPC(removec_craft_recipe_id, id);
}
Ref<CraftRecipe> Entity::gets_craft_recipe(int index) {
	ERR_FAIL_INDEX_V(index, _s_craft_recipes.size(), Ref<CraftRecipe>());

	return _s_craft_recipes.get(index);
}
Ref<CraftRecipe> Entity::gets_craft_recipe_id(int id) {
	for (int i = 0; i < _s_craft_recipes.size(); ++i) {
		Ref<CraftRecipe> craft_recipe = _s_craft_recipes.get(i);

		if (craft_recipe->get_id() == id) {
			return craft_recipe;
		}
	}

	return Ref<CraftRecipe>();
}
int Entity::gets_craft_recipe_count() {
	return _s_craft_recipes.size();
}

bool Entity::hasc_craft_recipe(Ref<CraftRecipe> craft_recipe) {
	for (int i = 0; i < _c_craft_recipes.size(); ++i) {
		if (_c_craft_recipes.get(i) == craft_recipe) {
			return true;
		}
	}

	return false;
}
bool Entity::hasc_craft_recipe_id(int id) {
	for (int i = 0; i < _c_craft_recipes.size(); ++i) {
		Ref<CraftRecipe> cr = _c_craft_recipes.get(i);

		ERR_CONTINUE(!cr.is_valid());

		if (cr->get_id() == id) {
			return true;
		}
	}

	return false;
}
void Entity::addc_craft_recipe(Ref<CraftRecipe> craft_recipe) {
	if (hasc_craft_recipe(craft_recipe))
		return;

	_c_craft_recipes.push_back(craft_recipe);

	emit_signal("ccraft_recipe_added", this, craft_recipe);
}
void Entity::addc_craft_recipe_id(int id) {
	ERR_FAIL_COND(!EntityDataManager::get_instance());

	if (hasc_craft_recipe_id(id))
		return;

	Ref<CraftRecipe> craft_recipe = EntityDataManager::get_instance()->get_craft_data(id);

	ERR_FAIL_COND(!craft_recipe.is_valid());

	_c_craft_recipes.push_back(craft_recipe);

	emit_signal("ccraft_recipe_added", this, craft_recipe);
}
void Entity::removec_craft_recipe(Ref<CraftRecipe> craft_recipe) {
	for (int i = 0; i < _c_craft_recipes.size(); ++i) {
		if (_c_craft_recipes.get(i) == craft_recipe) {
			_c_craft_recipes.remove(i);
			break;
		}
	}

	emit_signal("ccraft_recipe_removed", this, craft_recipe);
}
void Entity::removec_craft_recipe_id(int id) {
	Ref<CraftRecipe> craft_recipe;

	for (int i = 0; i < _c_craft_recipes.size(); ++i) {
		craft_recipe = _c_craft_recipes.get(i);

		if (craft_recipe->get_id() == id) {
			_c_craft_recipes.remove(i);
			break;
		}
	}

	emit_signal("ccraft_recipe_removed", this, craft_recipe);
}
Ref<CraftRecipe> Entity::getc_craft_recipe(int index) {
	ERR_FAIL_INDEX_V(index, _c_craft_recipes.size(), Ref<CraftRecipe>());

	return _c_craft_recipes.get(index);
}
int Entity::getc_craft_recipe_count() {
	return _c_craft_recipes.size();
}

////    Stat System    ////

Ref<Stat> Entity::get_stat_int(int index) {
	return _stats[index];
}

void Entity::set_stat_int(int index, Ref<Stat> entry) {
	_stats[index] = Ref<Stat>(entry);
}

Ref<Stat> Entity::get_stat_enum(Stat::StatId stat_id) {
	ERR_FAIL_INDEX_V(stat_id, Stat::STAT_ID_TOTAL_STATS, Ref<Stat>());

	return _stats[stat_id];
}

void Entity::set_stat_enum(Stat::StatId stat_id, Ref<Stat> entry) {
	ERR_FAIL_COND(!entry.is_valid());

	//ERR_FAIL_COND(stat_id == Stat::STAT_ID_NONE);

	if (stat_id == Stat::STAT_ID_NONE) {
		print_error("Add fail cond here, stat has STAT_ID_NONE!");
		_stats[0] = Ref<Stat>(entry);
		return;
	}

	_stats[stat_id] = Ref<Stat>(entry);
}

void Entity::sdie() {
	//serverside

	son_death();

	//send an event to client
	VRPC(cdie);

	//signal
	emit_signal("sdied", this);
}

void Entity::cdie() {
	con_death();

	emit_signal("cdied", this);
}

void Entity::ons_stat_changed(Ref<Stat> stat) {
	for (int i = 0; i < _s_resources.size(); ++i) {
		_s_resources.get(i)->ons_stat_changed(stat);
	}
}
void Entity::onc_stat_changed(Ref<Stat> stat) {
	for (int i = 0; i < _c_resources.size(); ++i) {
		_c_resources.get(i)->onc_stat_changed(stat);
	}
}

void Entity::ssend_stat(int id, int ccurrent, int cmax) {
	ERR_FAIL_INDEX(id, Stat::STAT_ID_TOTAL_STATS);

	if (id <= Stat::STAT_ID_MANA) {
		VRPC(creceive_stat, id, ccurrent, cmax);
		return;
	}

	ORPC(creceive_stat, id, ccurrent, cmax);
}

void Entity::creceive_stat(int id, int ccurrent, int cmax) {
	ERR_FAIL_INDEX(id, Stat::STAT_ID_TOTAL_STATS);

	_stats[id]->setc_values(ccurrent, cmax);
}

////    Equip Slots    ////

bool Entity::should_deny_equip(ItemEnums::EquipSlots equip_slot, Ref<ItemInstance> item) {
	if (_s_entity_data.is_valid()) {
		if (_s_entity_data->should_deny_equip(this, equip_slot, item))
			return true;
	}

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		if (ad->get_aura()->should_deny_equip(ad, equip_slot, item))
			return true;
	}

	if (has_method("_should_deny_equip"))
		if (call("_should_deny_equip", equip_slot, item))
			return true;

	return false;
}

void Entity::son_equip_success(ItemEnums::EquipSlots equip_slot, Ref<ItemInstance> item, Ref<ItemInstance> old_item, int bag_slot) {

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_equip_success(this, equip_slot, item, old_item, bag_slot);
	}

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_equip_success(ad, equip_slot, item, old_item, bag_slot);
	}

	if (has_method("_son_equip_success"))
		call("_son_equip_success", equip_slot, item, old_item, bag_slot);

	emit_signal("son_equip_success", this, equip_slot, item, old_item, bag_slot);
}

void Entity::son_equip_fail(ItemEnums::EquipSlots equip_slot, Ref<ItemInstance> item, Ref<ItemInstance> old_item, int bag_slot) {

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_equip_fail(this, equip_slot, item, old_item, bag_slot);
	}

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_equip_fail(ad, equip_slot, item, old_item, bag_slot);
	}

	if (has_method("_son_equip_fail"))
		call("_son_equip_fail", equip_slot, item, old_item, bag_slot);

	emit_signal("son_equip_fail", this, equip_slot, item, old_item, bag_slot);
}

void Entity::con_equip_success(ItemEnums::EquipSlots equip_slot, Ref<ItemInstance> item, Ref<ItemInstance> old_item, int bag_slot) {

	if (_c_entity_data.is_valid()) {
		_c_entity_data->con_equip_success(this, equip_slot, item, old_item, bag_slot);
	}

	for (int i = 0; i < _c_auras.size(); ++i) {
		Ref<AuraData> ad = _c_auras.get(i);

		ad->get_aura()->con_equip_success(ad, equip_slot, item, old_item, bag_slot);
	}

	if (has_method("_con_equip_success"))
		call("_con_equip_success", equip_slot, item, old_item, bag_slot);

	emit_signal("con_equip_success", this, equip_slot, item, old_item, bag_slot);
}

void Entity::con_equip_fail(ItemEnums::EquipSlots equip_slot, Ref<ItemInstance> item, Ref<ItemInstance> old_item, int bag_slot) {

	if (_c_entity_data.is_valid()) {
		_c_entity_data->con_equip_fail(this, equip_slot, item, old_item, bag_slot);
	}

	for (int i = 0; i < _c_auras.size(); ++i) {
		Ref<AuraData> ad = _c_auras.get(i);

		ad->get_aura()->con_equip_fail(ad, equip_slot, item, old_item, bag_slot);
	}

	if (has_method("_con_equip_fail"))
		call("_con_equip_fail", equip_slot, item, old_item, bag_slot);

	emit_signal("con_equip_fail", this, equip_slot, item, old_item, bag_slot);
}

void Entity::crequest_equip(ItemEnums::EquipSlots equip_slot, int bag_slot) {
	RPCS(sequip, equip_slot, bag_slot)
}
void Entity::sequip(ItemEnums::EquipSlots equip_slot, int bag_slot) {
	call("_sequip", equip_slot, bag_slot);
}
void Entity::_sequip(ItemEnums::EquipSlots equip_slot, int bag_slot) {
	ERR_FAIL_INDEX(equip_slot, ItemEnums::EQUIP_SLOT_EQUIP_SLOT_MAX);
	ERR_FAIL_COND(!_s_bag.is_valid());

	Ref<ItemInstance> bag_item = _s_bag->get_item(bag_slot);
	Ref<ItemInstance> equipped_item = gets_equip_slot(equip_slot);

	if (!can_equip_item(equip_slot, bag_item)) {
		ORPC(cequip_fail, equip_slot, bag_slot);
		return;
	}

	if (should_deny_equip(equip_slot, bag_item)) {
		ORPC(cequip_fail, equip_slot, bag_slot);
		return;
	}

	//check armor type
	//check required skills

	if (equipped_item.is_valid())
		sdeapply_item(equipped_item);

	if (bag_item.is_valid())
		sapply_item(bag_item);

	sets_equip_slot(equip_slot, bag_item);
	_s_bag->add_item_at(equip_slot, equipped_item, false);

	ORPC(cequip_success, equip_slot, bag_slot);
}
void Entity::cequip_success(ItemEnums::EquipSlots equip_slot, int bag_slot) {
	ERR_FAIL_INDEX(equip_slot, ItemEnums::EQUIP_SLOT_EQUIP_SLOT_MAX);
	ERR_FAIL_COND(!_c_bag.is_valid());

	Ref<ItemInstance> old_bag_item = _c_bag->get_item(bag_slot);
	Ref<ItemInstance> old_equipped_item = getc_equip_slot(equip_slot);

	_c_bag->add_item_at(bag_slot, old_equipped_item);
	setc_equip_slot(equip_slot, old_bag_item);

	if (old_equipped_item.is_valid())
		cdeapply_item(old_equipped_item);

	if (old_bag_item.is_valid())
		capply_item(old_bag_item);

	con_equip_success(equip_slot, old_bag_item, old_equipped_item, bag_slot);
}
void Entity::cequip_fail(ItemEnums::EquipSlots equip_slot, int bag_slot) {
	ERR_FAIL_INDEX(equip_slot, ItemEnums::EQUIP_SLOT_EQUIP_SLOT_MAX);
	ERR_FAIL_COND(!_c_bag.is_valid());

	Ref<ItemInstance> bag_item = _c_bag->get_item(bag_slot);
	Ref<ItemInstance> equipped_item = getc_equip_slot(equip_slot);

	con_equip_fail(equip_slot, equipped_item, bag_item, bag_slot);
}

Ref<ItemInstance> Entity::gets_equip_slot(int index) {
	ERR_FAIL_INDEX_V(index, ItemEnums::EQUIP_SLOT_EQUIP_SLOT_MAX, Ref<ItemInstance>());

	return _s_equipment[index];
}
void Entity::sets_equip_slot(int index, Ref<ItemInstance> item) {
	ERR_FAIL_INDEX(index, ItemEnums::EQUIP_SLOT_EQUIP_SLOT_MAX);

	_s_equipment[index] = item;
}

Ref<ItemInstance> Entity::getc_equip_slot(int index) {
	ERR_FAIL_INDEX_V(index, ItemEnums::EQUIP_SLOT_EQUIP_SLOT_MAX, Ref<ItemInstance>());

	return _c_equipment[index];
}
void Entity::setc_equip_slot(int index, Ref<ItemInstance> item) {
	ERR_FAIL_INDEX(index, ItemEnums::EQUIP_SLOT_EQUIP_SLOT_MAX);

	_c_equipment[index] = item;
}

bool Entity::can_equip_item(ItemEnums::EquipSlots equip_slot, Ref<ItemInstance> item) {
	return call("_can_equip_item", equip_slot, item);
}
bool Entity::_can_equip_item(ItemEnums::EquipSlots equip_slot, Ref<ItemInstance> item) {
	//deequip
	if (!item.is_valid())
		return true;

	Ref<ItemTemplate> it = item->get_item_template();

	ERR_FAIL_COND_V(!it.is_valid(), false);

	return it->get_equip_slot() == equip_slot;
}

void Entity::sapply_item(Ref<ItemInstance> item) {
	call("_sapply_item", item);
}
void Entity::sdeapply_item(Ref<ItemInstance> item) {
	call("_sdeapply_item", item);
}

void Entity::_sapply_item(Ref<ItemInstance> item) {
	ERR_FAIL_COND(!item.is_valid());

	Ref<ItemTemplate> it = item->get_item_template();

	ERR_FAIL_COND(!it.is_valid());

	for (int i = 0; i < item->get_item_stat_modifier_count(); ++i) {
		Ref<ItemStatModifier> mod = item->get_item_stat_modifier(i);

		if (!mod.is_valid())
			continue;

		Ref<Stat> stat = get_stat_enum(mod->get_stat_id());

		ERR_CONTINUE(!stat.is_valid());

		Ref<StatModifier> sm = stat->get_modifier(0);

		ERR_CONTINUE(!sm.is_valid());

		sm->set_base_mod(sm->get_base_mod() + mod->get_base_mod());
		sm->set_bonus_mod(sm->get_bonus_mod() + mod->get_bonus_mod());
		sm->set_percent_mod(sm->get_percent_mod() + mod->get_percent_mod());
	}
}
void Entity::_sdeapply_item(Ref<ItemInstance> item) {
	ERR_FAIL_COND(!item.is_valid());

	Ref<ItemTemplate> it = item->get_item_template();

	ERR_FAIL_COND(!it.is_valid());

	for (int i = 0; i < item->get_item_stat_modifier_count(); ++i) {
		Ref<ItemStatModifier> mod = item->get_item_stat_modifier(i);

		if (!mod.is_valid())
			continue;

		Ref<Stat> stat = get_stat_enum(mod->get_stat_id());

		ERR_CONTINUE(!stat.is_valid());

		Ref<StatModifier> sm = stat->get_modifier(0);

		ERR_CONTINUE(!sm.is_valid());

		sm->set_base_mod(sm->get_base_mod() - mod->get_base_mod());
		sm->set_bonus_mod(sm->get_bonus_mod() - mod->get_bonus_mod());
		sm->set_percent_mod(sm->get_percent_mod() - mod->get_percent_mod());
	}
}

void Entity::capply_item(Ref<ItemInstance> item) {
	call("_capply_item", item);
}
void Entity::cdeapply_item(Ref<ItemInstance> item) {
	call("_cdeapply_item", item);
}

void Entity::_capply_item(Ref<ItemInstance> item) {
	ERR_FAIL_COND(!item.is_valid());

	Ref<ItemTemplate> it = item->get_item_template();

	ERR_FAIL_COND(!it.is_valid());

	if (it->get_item_visual().is_valid() && ObjectDB::instance_validate(_character_skeleton)) {
		if (_character_skeleton->has_method("add_item_visual"))
			_character_skeleton->call("add_item_visual", it->get_item_visual());
	}
}
void Entity::_cdeapply_item(Ref<ItemInstance> item) {
	ERR_FAIL_COND(!item.is_valid());

	Ref<ItemTemplate> it = item->get_item_template();

	ERR_FAIL_COND(!it.is_valid());

	if (it->get_item_visual().is_valid() && ObjectDB::instance_validate(_character_skeleton)) {
		if (_character_skeleton->has_method("remove_item_visual"))
			_character_skeleton->call("remove_item_visual", it->get_item_visual());
	}
}

////    Resources    ////

Ref<EntityResource> Entity::gets_resource_index(int index) {
	ERR_FAIL_INDEX_V(index, _s_resources.size(), Ref<EntityResource>());

	return _s_resources.get(index);
}
Ref<EntityResource> Entity::gets_resource_id(int id) {
	for (int i = 0; i < _s_resources.size(); ++i) {
		Ref<EntityResource> r = _s_resources.get(i);

		if (r->get_data_id() == id) {
			return r;
		}
	}

	return Ref<EntityResource>();
}
void Entity::adds_resource(Ref<EntityResource> resource) {
	ERR_FAIL_COND(!resource.is_valid());

	_s_resources.push_back(resource);

	resource->ons_added(this);

	VRPCOBJP(addc_resource_rpc, _s_resources.size() - 1, JSON::print(resource->to_dict()), addc_resource, _s_resources.size() - 1, resource);
}
int Entity::gets_resource_count() {
	return _s_resources.size();
}
void Entity::removes_resource(int index) {
	ERR_FAIL_INDEX(index, _s_resources.size());

	_s_resources.remove(index);

	VRPC(removec_resource, index);
}
void Entity::clears_resource() {
	_s_resources.clear();

	VRPC(clearc_resource);
}

void Entity::addc_resource_rpc(int index, String data) {
	Ref<EntityResource> res;
	res.instance();
	res->from_dict(data_as_dict(data));
	res->resolve_references();

	addc_resource(index, res);
}

Ref<EntityResource> Entity::getc_resource_index(int index) {
	ERR_FAIL_INDEX_V(index, _c_resources.size(), Ref<EntityResource>());

	return _c_resources.get(index);
}
Ref<EntityResource> Entity::getc_resource_id(int id) {
	for (int i = 0; i < _c_resources.size(); ++i) {
		Ref<EntityResource> r = _c_resources.get(i);

		if (r->get_data_id() == id) {
			return r;
		}
	}

	return Ref<EntityResource>(NULL);
}
void Entity::addc_resource(int index, Ref<EntityResource> resource) {
	ERR_FAIL_COND(!resource.is_valid());

	if (_c_resources.size() <= index) {
		_c_resources.resize(index + 1);
	}

	_c_resources.set(index, resource);

	resource->onc_added(this);
}
int Entity::getc_resource_count() {
	return _c_resources.size();
}
void Entity::removec_resource(int index) {
	ERR_FAIL_INDEX(index, _c_resources.size());

	_c_resources.remove(index);
}
void Entity::clearc_resource() {
	_s_resources.clear();
}

void Entity::sends_resource_current(int index, int current) {
	VRPC(creceive_resource_current, index, current);
}
void Entity::sends_resource_curr_max(int index, int current, int max) {
	VRPC(creceive_resource_curr_max, index, current, max);
}
void Entity::sends_resource_data(int index, String data) {
	VRPC(creceive_resource_data, index, data);
}

void Entity::creceive_resource_current(int index, int current) {
	ERR_FAIL_INDEX(index, _c_resources.size());

	Ref<EntityResource> res = _c_resources.get(index);

	ERR_FAIL_COND(!res.is_valid());

	res->receivec_update(current);
}
void Entity::creceive_resource_curr_max(int index, int current, int max) {
	ERR_FAIL_INDEX(index, _c_resources.size());

	Ref<EntityResource> res = _c_resources.get(index);

	ERR_FAIL_COND(!res.is_valid());

	res->receivec_update_full(current, max);
}
void Entity::creceive_resource_data(int index, String data) {
	ERR_FAIL_INDEX(index, _c_resources.size());

	Ref<EntityResource> res = _c_resources.get(index);

	ERR_FAIL_COND(!res.is_valid());

	res->receivec_update_string(data);
}

void Entity::stake_damage(Ref<SpellDamageInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	//serverside

	if (gets_is_dead()) {
		return;
	}

	son_before_damage_hit(info);

	if (info->get_immune()) {
		VRPCOBJ(cdamage_dealt_rpc, JSON::print(info->to_dict()), con_damage_dealt, info);

		return;
	}

	//send it through the passive damage reductions pipeline
	sapply_passives_damage_receive(info);

	//send it through the onbeforehit handler
	son_before_damage(info);

	//send it throug the onhit pipeliine
	son_hit(info);

	son_damage_receive(info);

	int h = get_health()->gets_current() - info->get_damage();

	if (h < 0) {
		h = 0;
	}

	get_health()->sets_current(h);

	son_damage_dealt(info);

	//signal
	emit_signal("son_damage_received", this, info);

	//send an event to client
	VRPCOBJ(cdamage_dealt_rpc, JSON::print(info->to_dict()), con_damage_dealt, info);

	if (get_health()->gets_current() <= 0) {
		sdie();
	}
}

void Entity::sdeal_damage_to(Ref<SpellDamageInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	//serverside

	if (gets_is_dead()) {
		return;
	}

	sapply_passives_damage_deal(info);
	info->get_receiver()->stake_damage(info);
	son_dealt_damage(info);

	//send an event to client
	VRPCOBJ(cdealt_damage_rpc, JSON::print(info->to_dict()), con_dealt_damage, info);

	//signal
	emit_signal("son_damage_received", this, info);

	if (get_health()->gets_current() <= 0) {
		sdie();
	}
}

void Entity::stake_heal(Ref<SpellHealInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	//serverside

	if (gets_is_dead()) {
		return;
	}

	son_before_heal_hit(info);

	if (info->get_immune()) {
		VRPCOBJ(cheal_dealt_rpc, JSON::print(info->to_dict()), con_heal_dealt, info);
		return;
	}

	//send it through the passive damage reductions pipeline
	sapply_passives_heal_receive(info);

	//send it through the onbeforehit handler
	son_before_heal(info);

	son_heal_receive(info);

	int h = get_health()->gets_current() + info->get_heal();

	if (h > get_health()->gets_max()) {
		h = get_health()->gets_max();
	}
	get_health()->sets_current(h);

	//send an event to client
	VRPCOBJ(cheal_dealt_rpc, JSON::print(info->to_dict()), con_heal_dealt, info);

	//signal
	emit_signal("son_heal_received", this, info);
}

void Entity::sdeal_heal_to(Ref<SpellHealInfo> info) {
	ERR_FAIL_COND(!info.is_valid());
	ERR_FAIL_COND(info->get_receiver() == NULL);

	//serverside

	if (gets_is_dead()) {
		return;
	}

	sapply_passives_heal_deal(info);
	info->get_receiver()->stake_heal(info);
	son_heal_dealt(info);

	VRPCOBJ(cdealt_heal_rpc, JSON::print(info->to_dict()), con_dealt_heal, info);

	emit_signal("son_heal_dealt", this, info);
}

//Damage, Heal RPCs
void Entity::cdamage_dealt_rpc(String data) {
	Ref<SpellDamageInfo> info;
	info.instance();
	info->from_dict(data_as_dict(data));
	info->resolve_references(this);

	con_damage_dealt(info);
}
void Entity::cdealt_damage_rpc(String data) {
	Ref<SpellDamageInfo> info;
	info.instance();
	info->from_dict(data_as_dict(data));
	info->resolve_references(this);

	con_dealt_damage(info);
}
void Entity::cheal_dealt_rpc(String data) {
	Ref<SpellHealInfo> info;
	info.instance();
	info->from_dict(data_as_dict(data));
	info->resolve_references(this);

	con_heal_dealt(info);
}
void Entity::cdealt_heal_rpc(String data) {
	Ref<SpellHealInfo> info;
	info.instance();
	info->from_dict(data_as_dict(data));
	info->resolve_references(this);

	con_dealt_heal(info);
}

//Interactions
bool Entity::cans_interact() {
	if (!_s_entity_data.is_valid()) {
		if (has_method("_cans_interact")) {
			return call("_cans_interact");
		}

		return false;
	}

	return _s_entity_data->cans_interact(this);
}

void Entity::sinteract() {
	if (!cans_interact()) {
		return;
	}

	if (!_s_entity_data.is_valid()) {
		if (has_method("_sinteract")) {
			call("_sinteract");
		}
	}

	_s_entity_data->sinteract(this);
}

bool Entity::canc_interact() {
	if (has_method("_canc_interact")) {
		return call("_canc_interact");
	}

	if (!ObjectDB::instance_validate(_c_target)) {
		return false;
	}

	EntityEnums::EntityInteractionType it = _c_target->getc_entity_interaction_type();

	if (it == EntityEnums::ENITIY_INTERACTION_TYPE_NONE || it == EntityEnums::ENITIY_INTERACTION_TYPE_NORMAL) {
		return false;
	}

	return true;
}

void Entity::crequest_interact() {
	sinteract();
}

void Entity::ssend_open_window(int window_id) {
	ORPC(copen_window, window_id);
}
void Entity::copen_window(int window_id) {
	switch (window_id) {
		case EntityEnums::ENTITY_WINDOW_LOOT:
			emit_signal("onc_open_loot_winow_request");
			break;
		case EntityEnums::ENTITY_WINDOW_CONTAINER:
			emit_signal("onc_open_container_winow_request");
			break;
		case EntityEnums::ENTITY_WINDOW_VENDOR:
			emit_signal("onc_open_vendor_winow_request");
			break;
	}
}

//XP Operations
void Entity::adds_xp(int value) {
	_s_xp += value;

	son_xp_gained(value);

	ORPC(addc_xp, value);
}
void Entity::addc_xp(int value) {
	_c_xp += value;

	con_xp_gained(value);
}

void Entity::slevelup(int value) {
	if (_s_level == EntityEnums::MAX_LEVEL)
		return;

	_s_level += value;

	son_level_up(value);

	VRPC(clevelup, value);
}
void Entity::clevelup(int value) {
	_c_level += value;

	con_level_up(value);
}

void Entity::son_before_aura_applied(Ref<AuraData> data) {
	ERR_FAIL_COND(!data.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_after_aura_applied(data);
	}

	if (has_method("_son_before_aura_applied"))
		call("_son_before_aura_applied", data);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_before_aura_applied(data);
	}
}

void Entity::son_after_aura_applied(Ref<AuraData> data) {
	ERR_FAIL_COND(!data.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_after_aura_applied(data);
	}

	if (has_method("_son_after_aura_applied"))
		call("_son_after_aura_applied", data);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		if (ad == data) {
			continue;
		}

		ad->get_aura()->son_after_aura_applied(data);
	}
}

////    Spell System    ////

void Entity::scast_spell(int spell_id) {
	Ref<EntityData> cc = gets_entity_data();

	if (cc == NULL)
		return;

	cc->start_casting(spell_id, this, 1);
}

void Entity::crequest_spell_cast(int spell_id) {
	RPCS(scast_spell, spell_id);
}

void Entity::update_auras(float delta) {
	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->supdate(ad, delta);
	}
}

void Entity::son_before_cast(Ref<SpellCastInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_before_cast(info);
	}

	if (has_method("_son_before_cast"))
		call("_son_before_cast", info);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_before_cast(ad, info);
	}
}

void Entity::son_before_cast_target(Ref<SpellCastInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_after_aura_applied(info);
	}

	if (has_method("_son_before_cast_target"))
		call("_son_before_cast_target", info);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_before_cast_target(ad, info);
	}
}

void Entity::son_before_damage_hit(Ref<SpellDamageInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_before_damage_hit(info);
	}

	if (has_method("_son_before_damage_hit"))
		call("_son_before_damage_hit", info);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_before_damage_hit(ad, info);
	}
}

void Entity::son_hit(Ref<SpellDamageInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_hit(info);
	}

	if (has_method("_son_hit"))
		call("_son_hit", info);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_hit(ad, info);
	}
}

void Entity::son_before_damage(Ref<SpellDamageInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_before_damage(info);
	}

	if (has_method("_son_before_damage"))
		call("_son_before_damage", info);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_before_damage(ad, info);
	}
}

void Entity::son_damage_receive(Ref<SpellDamageInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_damage_receive(info);
	}

	if (has_method("_son_damage_receive"))
		call("_son_damage_receive", info);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_damage_receive(ad, info);
	}
}

void Entity::son_dealt_damage(Ref<SpellDamageInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_dealt_damage(info);
	}

	if (has_method("_son_dealt_damage"))
		call("_son_dealt_damage", info);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_dealt_damage(ad, info);
	}
}

void Entity::son_damage_dealt(Ref<SpellDamageInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_damage_dealt(info);
	}

	if (has_method("_son_damage_dealt"))
		call("_son_damage_dealt", info);

	//serverside
	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_damage_dealt(ad, info);
	}
}

void Entity::son_before_heal_hit(Ref<SpellHealInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_before_heal_hit(info);
	}

	if (has_method("_son_before_heal_hit"))
		call("_son_before_heal_hit", info);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_before_heal_hit(ad, info);
	}
}

void Entity::son_before_heal(Ref<SpellHealInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_before_heal(info);
	}

	if (has_method("_son_before_heal"))
		call("_son_before_heal", info);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_before_heal(ad, info);
	}
}

void Entity::son_heal_receive(Ref<SpellHealInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_heal_receive(info);
	}

	if (has_method("_son_heal_receive"))
		call("_son_heal_receive", info);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_heal_receive(ad, info);
	}
}

void Entity::son_dealt_heal(Ref<SpellHealInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_dealt_heal(info);
	}

	if (has_method("_son_dealt_heal"))
		call("_son_dealt_heal", info);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_dealt_heal(ad, info);
	}
}

void Entity::son_heal_dealt(Ref<SpellHealInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_heal_dealt(info);
	}

	if (has_method("_son_heal_dealt"))
		call("_son_heal_dealt", info);

	//serverside
	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_heal_dealt(ad, info);
	}
}

void Entity::sapply_passives_damage_receive(Ref<SpellDamageInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->sapply_passives_damage_receive(info);
	}
}

void Entity::sapply_passives_damage_deal(Ref<SpellDamageInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->sapply_passives_damage_deal(info);
	}
}

void Entity::sapply_passives_heal_receive(Ref<SpellHealInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->sapply_passives_heal_receive(info);
	}
}

void Entity::sapply_passives_heal_deal(Ref<SpellHealInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->sapply_passives_heal_deal(info);
	}
}

void Entity::son_cast_finished(Ref<SpellCastInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_cast_finished(info);
	}

	if (has_method("_son_cast_finished"))
		call("_son_cast_finished", info);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_cast_finished(ad, info);
	}
}

void Entity::son_cast_started(Ref<SpellCastInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_cast_started(info);
	}

	if (has_method("_son_cast_started"))
		call("_son_cast_started", info);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_cast_started(ad, info);
	}
}

void Entity::son_cast_failed(Ref<SpellCastInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_cast_failed(info);
	}

	if (has_method("_son_cast_failed"))
		call("_son_cast_failed", info);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_cast_failed(ad, info);
	}
}

void Entity::son_cast_finished_target(Ref<SpellCastInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_cast_finished_target(info);
	}

	if (has_method("_son_cast_finished_target"))
		call("_son_cast_finished_target", info);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_cast_finished_target(ad, info);
	}
}

void Entity::son_spell_cast_success(Ref<SpellCastInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_spell_cast_success(info);
	}

	if (has_method("_son_spell_cast_success"))
		call("_son_spell_cast_success", info);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_spell_cast_success(ad, info);
	}

	emit_signal("sspell_cast_success", info);
}

void Entity::son_death() {
	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_death(this);
	}

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_death(ad);
	}

	if (has_method("_son_death"))
		call("_son_death");
}

void Entity::son_cooldown_added(Ref<Cooldown> cooldown) {
	ERR_FAIL_COND(!cooldown.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_cooldown_added(cooldown);
	}

	if (has_method("_son_cooldown_added"))
		call("_son_cooldown_added", cooldown);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_cooldown_added(ad, cooldown);
	}
}
void Entity::son_cooldown_removed(Ref<Cooldown> cooldown) {
	ERR_FAIL_COND(!cooldown.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_cooldown_removed(cooldown);
	}

	if (has_method("_son_cooldown_removed"))
		call("_son_cooldown_removed", cooldown);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_cooldown_removed(ad, cooldown);
	}
}

void Entity::son_category_cooldown_added(Ref<CategoryCooldown> category_cooldown) {
	ERR_FAIL_COND(!category_cooldown.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_category_cooldown_added(category_cooldown);
	}

	if (has_method("_son_category_cooldown_added"))
		call("_son_category_cooldown_added", category_cooldown);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_category_cooldown_added(ad, category_cooldown);
	}
}
void Entity::son_category_cooldown_removed(Ref<CategoryCooldown> category_cooldown) {
	ERR_FAIL_COND(!category_cooldown.is_valid());

	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_category_cooldown_removed(category_cooldown);
	}

	if (has_method("_son_category_cooldown_removed"))
		call("_son_category_cooldown_removed", category_cooldown);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_category_cooldown_removed(ad, category_cooldown);
	}
}

void Entity::son_gcd_started() {
	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_gcd_started(this, _s_gcd);
	}

	if (has_method("_son_gcd_started"))
		call("_son_gcd_started", _s_gcd);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_gcd_started(ad, _s_gcd);
	}
}
void Entity::son_gcd_finished() {
	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_gcd_finished(this);
	}

	if (has_method("_son_gcd_finished"))
		call("_son_gcd_finished");

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_gcd_finished(ad);
	}
}
void Entity::con_gcd_started() {
	if (_s_entity_data.is_valid()) {
		_s_entity_data->con_gcd_started(this, _c_gcd);
	}

	if (has_method("_con_gcd_started"))
		call("_con_gcd_started", _c_gcd);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->con_gcd_started(ad, _c_gcd);
	}
}
void Entity::con_gcd_finished() {
	if (_s_entity_data.is_valid()) {
		_s_entity_data->con_gcd_finished(this);
	}

	if (has_method("_con_gcd_finished"))
		call("_con_gcd_finished");

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->con_gcd_finished(ad);
	}
}

void Entity::son_physics_process(float delta) {
	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_physics_process(ad);
	}

	if (_physics_process_scis.size() > 0) {
		for (int i = 0; i < _physics_process_scis.size(); ++i) {
			Ref<SpellCastInfo> sci = _physics_process_scis.get(i);

			ERR_CONTINUE(!sci.is_valid());

			sci->physics_process(delta);
		}

		_physics_process_scis.clear();
	}
}

void Entity::son_xp_gained(int value) {
	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_xp_gained(this, value);
	}

	if (has_method("_son_xp_gained"))
		call("_son_xp_gained", value);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_xp_gained(ad, value);
	}

	emit_signal("son_xp_gained", this, value);
}

void Entity::son_level_up(int value) {
	if (_s_entity_data.is_valid()) {
		_s_entity_data->son_level_up(this, value);
	}

	if (has_method("_son_level_up"))
		call("_son_level_up", value);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_level_up(ad, value);
	}

	emit_signal("son_level_up", this, value);
}

void Entity::sadd_aura(Ref<AuraData> aura) {
	ERR_FAIL_COND(!aura.is_valid());

	son_before_aura_applied(aura);

	aura->set_owner(this);

	_s_auras.push_back(aura);

	son_after_aura_applied(aura);

	emit_signal("saura_added", aura);

	if (!aura->get_aura()->get_hide())
		VRPCOBJ(cadd_aura_rpc, JSON::print(aura->to_dict()), cadd_aura, aura);
}

void Entity::sremove_aura(Ref<AuraData> aura) {
	ERR_FAIL_COND(!aura.is_valid());

	int aid = aura->get_aura_id();
	Entity *caster = aura->get_caster();

	Ref<AuraData> a;
	bool removed = false;
	for (int i = 0; i < _s_auras.size(); i++) {
		a = _s_auras.get(i);

		if (a->get_aura_id() == aid && a->get_caster() == caster) {
			_s_auras.remove(i);
			removed = true;
			break;
		}
	}

	if (removed) {
		emit_signal("saura_removed", a);

		if (!aura->get_aura()->get_hide())
			VRPCOBJ(cremove_aura_rpc, JSON::print(aura->to_dict()), cremove_aura, aura);
	}
}

void Entity::sremove_aura_exact(Ref<AuraData> aura) {
	ERR_FAIL_COND(!aura.is_valid());

	for (int i = 0; i < _s_auras.size(); i++) {
		Ref<AuraData> ad = _s_auras.get(i);

		if (ad == aura) {
			ad->get_aura()->son_remove(ad);

			_s_auras.remove(i);

			break;
		}
	}

	emit_signal("saura_removed", aura);

	if (!aura->get_aura()->get_hide())
		VRPCOBJ(cremove_aura_rpc, JSON::print(aura->to_dict()), cremove_aura, aura);
}

void Entity::sremove_aura_expired(Ref<AuraData> aura) {
	ERR_FAIL_COND(!aura.is_valid());

	for (int i = 0; i < _s_auras.size(); i++) {
		Ref<AuraData> ad = _s_auras.get(i);

		if (ad == aura) {
			ad->get_aura()->son_remove(ad);

			_s_auras.remove(i);

			break;
		}
	}

	emit_signal("saura_removed_expired", aura);

	if (!aura->get_aura()->get_hide())
		VRPCOBJ(cremove_aura_rpc, JSON::print(aura->to_dict()), cremove_aura, aura);
}

void Entity::sremove_aura_dispelled(Ref<AuraData> aura) {
	ERR_FAIL_COND(!aura.is_valid());

	for (int i = 0; i < _s_auras.size(); i++) {
		Ref<AuraData> ad = _s_auras.get(i);

		if (ad == aura) {
			ad->get_aura()->son_remove(ad);

			_s_auras.remove(i);

			break;
		}
	}

	emit_signal("saura_removed_dispelled", aura);

	if (!aura->get_aura()->get_hide())
		VRPCOBJ(cremove_aura_rpc, JSON::print(aura->to_dict()), cremove_aura, aura);
}

void Entity::saura_refreshed(Ref<AuraData> aura) {
	//ERR_EXPLAIN("NYI");
	ERR_FAIL();

	ERR_FAIL_COND(!aura.is_valid());

	emit_signal("caura_refreshed", aura);

	if (!aura->get_aura()->get_hide())
		VRPCOBJ(caura_refreshed_rpc, JSON::print(aura->to_dict()), caura_refreshed, aura);
}

void Entity::cadd_aura_rpc(String data) {
	Ref<AuraData> aura;
	aura.instance();
	aura->from_dict(data_as_dict(data));
	aura->set_owner(this);
	aura->resolve_references(this);

	cadd_aura(aura);
}

void Entity::cremove_aura_rpc(String data) {
	Ref<AuraData> aura;
	aura.instance();
	aura->from_dict(data_as_dict(data));
	aura->set_owner(this);
	aura->resolve_references(this);

	cremove_aura(aura);
}

void Entity::cremove_aura_exact_rpc(String data) {
	Ref<AuraData> aura;
	aura.instance();
	aura->from_dict(data_as_dict(data));
	aura->set_owner(this);
	aura->resolve_references(this);

	cremove_aura_exact(aura);
}

void Entity::cremove_aura_expired_rpc(String data) {
	Ref<AuraData> aura;
	aura.instance();
	aura->from_dict(data_as_dict(data));
	aura->set_owner(this);
	aura->resolve_references(this);

	cremove_aura_expired(aura);
}

void Entity::cremove_aura_dispelled_rpc(String data) {
	Ref<AuraData> aura;
	aura.instance();
	aura->from_dict(data_as_dict(data));
	aura->set_owner(this);
	aura->resolve_references(this);

	cremove_aura_dispelled(aura);
}

void Entity::caura_refreshed_rpc(String data) {
	Ref<AuraData> aura;
	aura.instance();
	aura->from_dict(data_as_dict(data));
	aura->set_owner(this);
	aura->resolve_references(this);

	caura_refreshed(aura);
}

void Entity::cadd_aura(Ref<AuraData> aura) {
	ERR_FAIL_COND(!aura.is_valid());

	_c_auras.push_back(aura);

	con_aura_added(aura);

	emit_signal("caura_added", aura);
}

void Entity::cremove_aura(Ref<AuraData> aura) {
	ERR_FAIL_COND(!aura.is_valid());

	int aid = aura->get_aura_id();
	Entity *caster = aura->get_caster();

	Ref<AuraData> a;
	bool removed = false;
	for (int i = 0; i < _c_auras.size(); i++) {
		a = _c_auras.get(i);

		if (a->get_aura_id() == aid && a->get_caster() == caster) {
			_c_auras.remove(i);
			removed = true;
			break;
		}
	}

	if (removed) {
		con_aura_removed(aura);

		emit_signal("caura_removed", a);
	}
}

void Entity::cremove_aura_exact(Ref<AuraData> aura) {
	ERR_FAIL_COND(!aura.is_valid());

	for (int i = 0; i < _c_auras.size(); i++) {
		if (_c_auras.get(i) == aura) {
			_c_auras.remove(i);
			break;
		}
	}

	con_aura_removed(aura);

	emit_signal("caura_removed", aura);
}

void Entity::cremove_aura_dispelled(Ref<AuraData> aura) {
	ERR_FAIL_COND(!aura.is_valid());

	for (int i = 0; i < _c_auras.size(); i++) {
		if (_c_auras.get(i) == aura) {
			_c_auras.remove(i);
			break;
		}
	}

	con_aura_removed(aura);

	emit_signal("caura_removed_dispelled", aura);
}

void Entity::caura_refreshed(Ref<AuraData> aura) {
	//ERR_EXPLAIN("NYI");
	ERR_FAIL();

	ERR_FAIL_COND(!aura.is_valid());

	emit_signal("caura_refreshed", aura);
}

void Entity::cremove_aura_expired(Ref<AuraData> aura) {
	ERR_FAIL_COND(!aura.is_valid());

	for (int i = 0; i < _c_auras.size(); i++) {
		if (_c_auras.get(i) == aura) {
			_c_auras.remove(i);
			break;
		}
	}

	con_aura_removed(aura);

	emit_signal("caura_removed_expired", aura);
}

int Entity::sget_aura_count() {
	return _s_auras.size();
}

Ref<AuraData> Entity::sget_aura(int index) {
	ERR_FAIL_INDEX_V(index, _s_auras.size(), Ref<AuraData>(NULL));

	return Ref<AuraData>(_s_auras.get(index));
}

Ref<AuraData> Entity::sget_aura_by(Entity *caster, int aura_id) {
	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		if (ad->get_aura_id() == aura_id && ad->get_caster() == caster) {
			return ad;
		}
	}

	return Ref<AuraData>(NULL);
}
Ref<AuraData> Entity::sget_aura_by_bind(Node *caster, int aura_id) {
	if (!caster) {
		return Ref<AuraData>();
	}

	Entity *e = cast_to<Entity>(caster);

	if (!e) {
		return Ref<AuraData>();
	}

	return sget_aura_by(e, aura_id);
}

Ref<AuraData> Entity::sget_aura_with_group_by(Entity *caster, Ref<AuraGroup> aura_group) {
	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		if (ad->get_aura()->get_aura_group() == aura_group && ad->get_caster() == caster) {
			return ad;
		}
	}

	return Ref<AuraData>();
}
Ref<AuraData> Entity::sget_aura_with_group_by_bind(Node *caster, Ref<AuraGroup> aura_group) {
	if (!ObjectDB::instance_validate(caster)) {
		return Ref<AuraData>();
	}

	Entity *e = cast_to<Entity>(caster);

	if (!e) {
		return Ref<AuraData>();
	}

	return sget_aura_with_group_by(e, aura_group);
}

int Entity::cget_aura_count() {
	return _s_auras.size();
}

Ref<AuraData> Entity::cget_aura(int index) {
	ERR_FAIL_INDEX_V(index, _c_auras.size(), Ref<AuraData>(NULL));

	return Ref<AuraData>(_c_auras.get(index));
}

void Entity::moved() {
	if (has_method("_moved"))
		call("_moved");
}

void Entity::onc_mouse_enter() {
	if (has_method("_onc_mouse_enter"))
		call("_onc_mouse_enter");

	emit_signal("onc_mouse_entered");
}
void Entity::onc_mouse_exit() {
	if (has_method("_onc_mouse_exit"))
		call("_onc_mouse_exit");

	emit_signal("onc_mouse_exited");
}

void Entity::onc_targeted() {
	if (has_method("_onc_targeted"))
		call("_onc_targeted");

	emit_signal("onc_targeted");
}
void Entity::onc_untargeted() {
	if (has_method("_onc_untargeted"))
		call("_onc_untargeted");

	emit_signal("onc_untargeted");
}

void Entity::con_cast_failed(Ref<SpellCastInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	info->get_spell()->con_spell_cast_failed(info);

	if (_c_entity_data.is_valid()) {
		_c_entity_data->con_cast_failed(info);
	}

	for (int i = 0; i < _c_auras.size(); ++i) {
		Ref<AuraData> ad = _c_auras.get(i);

		ad->get_aura()->con_cast_failed(ad, info);
	}

	if (has_method("_con_cast_failed"))
		call("_con_cast_failed", info);
}

void Entity::con_cast_started(Ref<SpellCastInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	info->get_spell()->con_spell_cast_started(info);

	if (_c_entity_data.is_valid()) {
		_c_entity_data->con_cast_started(info);
	}

	for (int i = 0; i < _c_auras.size(); ++i) {
		Ref<AuraData> ad = _c_auras.get(i);

		ad->get_aura()->con_cast_started(ad, info);
	}

	if (has_method("_con_cast_started"))
		call("_con_cast_started", info);
}

void Entity::con_cast_state_changed(Ref<SpellCastInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	//info->get_spell()->con_spell_cast_(info);

	if (_c_entity_data.is_valid()) {
		_c_entity_data->con_cast_state_changed(info);
	}

	for (int i = 0; i < _c_auras.size(); ++i) {
		Ref<AuraData> ad = _c_auras.get(i);

		ad->get_aura()->con_cast_state_changed(ad, info);
	}

	if (has_method("_con_cast_state_changed"))
		call("_con_cast_state_changed", info);
}

void Entity::con_cast_finished(Ref<SpellCastInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	info->get_spell()->con_spell_cast_success(info);

	if (_c_entity_data.is_valid()) {
		_c_entity_data->con_cast_finished(info);
	}

	for (int i = 0; i < _c_auras.size(); ++i) {
		Ref<AuraData> ad = _c_auras.get(i);

		ad->get_aura()->con_cast_finished(ad, info);
	}

	if (has_method("_con_cast_finished"))
		call("_con_cast_finished", info);
}

void Entity::con_spell_cast_success(Ref<SpellCastInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	info->get_spell()->con_spell_cast_success(info);

	if (_c_entity_data.is_valid()) {
		_c_entity_data->con_spell_cast_success(info);
	}

	for (int i = 0; i < _c_auras.size(); ++i) {
		Ref<AuraData> ad = _c_auras.get(i);

		ad->get_aura()->con_spell_cast_success(ad, info);
	}

	if (has_method("_con_spell_cast_success"))
		call("_con_spell_cast_success", info);

	emit_signal("cspell_cast_success", info);
}

void Entity::con_death() {
	if (_c_entity_data.is_valid()) {
		_c_entity_data->con_death(this);
	}

	for (int i = 0; i < _c_auras.size(); ++i) {
		Ref<AuraData> ad = _c_auras.get(i);

		ad->get_aura()->con_death(ad);
	}

	if (has_method("_con_death"))
		call("_con_death");
}

void Entity::con_cooldown_added(Ref<Cooldown> cooldown) {
	ERR_FAIL_COND(!cooldown.is_valid());

	if (_c_entity_data.is_valid()) {
		_c_entity_data->con_cooldown_added(cooldown);
	}

	for (int i = 0; i < _c_auras.size(); ++i) {
		Ref<AuraData> ad = _c_auras.get(i);

		ad->get_aura()->con_cooldown_added(ad, cooldown);
	}

	if (has_method("_con_cooldown_added"))
		call("_con_cooldown_added", cooldown);
}
void Entity::con_cooldown_removed(Ref<Cooldown> cooldown) {
	ERR_FAIL_COND(!cooldown.is_valid());

	if (_c_entity_data.is_valid()) {
		_c_entity_data->con_cooldown_removed(cooldown);
	}

	for (int i = 0; i < _c_auras.size(); ++i) {
		Ref<AuraData> ad = _c_auras.get(i);

		ad->get_aura()->con_cooldown_removed(ad, cooldown);
	}

	if (has_method("_con_cooldown_removed"))
		call("_con_cooldown_removed", cooldown);
}
void Entity::con_category_cooldown_added(Ref<CategoryCooldown> category_cooldown) {
	ERR_FAIL_COND(!category_cooldown.is_valid());

	if (_c_entity_data.is_valid()) {
		_c_entity_data->con_category_cooldown_added(category_cooldown);
	}

	for (int i = 0; i < _c_auras.size(); ++i) {
		Ref<AuraData> ad = _c_auras.get(i);

		ad->get_aura()->con_category_cooldown_added(ad, category_cooldown);
	}

	if (has_method("_con_category_cooldown_added"))
		call("_con_category_cooldown_added", category_cooldown);
}
void Entity::con_category_cooldown_removed(Ref<CategoryCooldown> category_cooldown) {
	ERR_FAIL_COND(!category_cooldown.is_valid());

	if (_c_entity_data.is_valid()) {
		_c_entity_data->con_category_cooldown_removed(category_cooldown);
	}

	for (int i = 0; i < _c_auras.size(); ++i) {
		Ref<AuraData> ad = _c_auras.get(i);

		ad->get_aura()->con_category_cooldown_removed(ad, category_cooldown);
	}

	if (has_method("_con_category_cooldown_removed"))
		call("_con_category_cooldown_removed", category_cooldown);
}

void Entity::con_aura_added(Ref<AuraData> data) {
	ERR_FAIL_COND(!data.is_valid());

	if (_c_entity_data.is_valid()) {
		_c_entity_data->con_aura_added(data);
	}

	for (int i = 0; i < _c_auras.size(); ++i) {
		Ref<AuraData> ad = _c_auras.get(i);

		ad->get_aura()->con_aura_added(data);
	}

	if (has_method("_con_aura_added"))
		call("_con_aura_added", data);
}

void Entity::con_aura_removed(Ref<AuraData> data) {
	ERR_FAIL_COND(!data.is_valid());

	if (_c_entity_data.is_valid()) {
		_c_entity_data->con_aura_removed(data);
	}

	for (int i = 0; i < _c_auras.size(); ++i) {
		Ref<AuraData> ad = _c_auras.get(i);

		ad->get_aura()->con_aura_removed(data);
	}

	if (has_method("_con_aura_removed"))
		call("_con_aura_removed", data);
}

void Entity::con_aura_refresh(Ref<AuraData> data) {
	ERR_FAIL_COND(!data.is_valid());

	if (_c_entity_data.is_valid()) {
		_c_entity_data->con_aura_refresh(data);
	}

	for (int i = 0; i < _c_auras.size(); ++i) {
		Ref<AuraData> ad = _c_auras.get(i);

		ad->get_aura()->con_aura_refresh(data);
	}

	if (has_method("_con_aura_refresh"))
		call("_con_aura_refresh", data);
}

void Entity::con_damage_dealt(Ref<SpellDamageInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_c_entity_data.is_valid()) {
		_c_entity_data->con_damage_dealt(info);
	}

	for (int i = 0; i < _c_auras.size(); ++i) {
		Ref<AuraData> ad = _c_auras.get(i);

		ad->get_aura()->con_damage_dealt(ad, info);
	}

	if (has_method("_con_damage_dealt"))
		call("_con_damage_dealt", info);

	//the current c health should probably be set here.
	emit_signal("con_damage_dealt", this, info);
}

void Entity::con_dealt_damage(Ref<SpellDamageInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_c_entity_data.is_valid()) {
		_c_entity_data->con_dealt_damage(info);
	}

	for (int i = 0; i < _c_auras.size(); ++i) {
		Ref<AuraData> ad = _c_auras.get(i);

		ad->get_aura()->con_dealt_damage(ad, info);
	}

	if (has_method("_con_dealt_damage"))
		call("_con_dealt_damage", info);

	//the current c health should probably be set here.
	emit_signal("con_dealt_damage", this, info);
}

void Entity::con_heal_dealt(Ref<SpellHealInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_c_entity_data.is_valid()) {
		_c_entity_data->con_heal_dealt(info);
	}

	for (int i = 0; i < _c_auras.size(); ++i) {
		Ref<AuraData> ad = _c_auras.get(i);

		ad->get_aura()->con_heal_dealt(ad, info);
	}

	if (has_method("_con_heal_dealt"))
		call("_con_heal_dealt", info);

	//the current c health should probably be set here.
	emit_signal("con_heal_dealt", this, info);
}

void Entity::con_dealt_heal(Ref<SpellHealInfo> info) {
	ERR_FAIL_COND(!info.is_valid());

	if (_c_entity_data.is_valid()) {
		_c_entity_data->con_dealt_heal(info);
	}

	for (int i = 0; i < _c_auras.size(); ++i) {
		Ref<AuraData> ad = _c_auras.get(i);

		ad->get_aura()->con_dealt_heal(ad, info);
	}

	if (has_method("_con_dealt_heal"))
		call("_con_dealt_heal", info);

	//the current c health should probably be set here.
	emit_signal("con_dealt_heal", this, info);
}

void Entity::con_xp_gained(int value) {
	if (_s_entity_data.is_valid()) {
		_s_entity_data->con_xp_gained(this, value);
	}

	if (has_method("_con_xp_gained"))
		call("_con_xp_gained", value);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->con_xp_gained(ad, value);
	}

	emit_signal("con_xp_gained", this, value);
}

void Entity::con_level_up(int value) {
	if (_s_entity_data.is_valid()) {
		_s_entity_data->con_level_up(this, value);
	}

	if (has_method("_con_level_up"))
		call("_con_level_up", value);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->con_level_up(ad, value);
	}

	emit_signal("con_level_up", this, value);
}

////    Casting System    ////

void Entity::sstart_casting(Ref<SpellCastInfo> info) {
	_s_spell_cast_info = Ref<SpellCastInfo>(info);

	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_before_cast(ad, info);
	}

	_s_spell_cast_info->set_is_casting(true);

	emit_signal("scast_started", info);

	VRPCOBJ(cstart_casting_rpc, JSON::print(info->to_dict()), cstart_casting, info);
}

void Entity::sfail_cast() {
	son_cast_failed(_s_spell_cast_info);

	emit_signal("scast_failed", _s_spell_cast_info);

	_s_spell_cast_info.unref();

	VRPC(cfail_cast);
}

void Entity::sdelay_cast() {
	emit_signal("scast_delayed", _s_spell_cast_info);

	VRPC(cdelay_cast);
}

void Entity::sfinish_cast() {
	_s_spell_cast_info->get_spell()->sfinish_cast(_s_spell_cast_info);

	son_cast_finished(_s_spell_cast_info);

	emit_signal("scast_finished", _s_spell_cast_info);

	_s_spell_cast_info.unref();

	VRPC(cfinish_cast);
}

void Entity::sinterrupt_cast() {
	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		ad->get_aura()->son_before_cast(ad, _s_spell_cast_info);
	}

	emit_signal("scast_interrupted", _s_spell_cast_info);

	_s_spell_cast_info.unref();

	VRPC(cinterrupt_cast);
}

void Entity::cstart_casting_rpc(String data) {
	Ref<SpellCastInfo> info;
	info.instance();
	info->from_dict(data_as_dict(data));
	info->resolve_references(this);

	cstart_casting(info);
}

void Entity::cstart_casting(Ref<SpellCastInfo> info) {
	_c_spell_cast_info = Ref<SpellCastInfo>(info);

	con_cast_started(_c_spell_cast_info);

	emit_signal("ccast_started", _c_spell_cast_info);
}

void Entity::cfail_cast() {
	con_cast_failed(_c_spell_cast_info);

	emit_signal("ccast_failed", _c_spell_cast_info);

	_c_spell_cast_info.unref();
}

void Entity::cdelay_cast() {

	//c_on_cast_

	emit_signal("ccast_delayed", _c_spell_cast_info);
}

void Entity::cfinish_cast() {
	con_cast_finished(_c_spell_cast_info);
	emit_signal("ccast_finished", _c_spell_cast_info);
	_c_spell_cast_info.unref();
}

void Entity::cinterrupt_cast() {
	con_cast_failed(_c_spell_cast_info);
	emit_signal("ccast_interrupted", _c_spell_cast_info);
	_c_spell_cast_info.unref();
}

void Entity::sspell_cast_success(Ref<SpellCastInfo> info) {
	son_spell_cast_success(info);

	VRPCOBJ(cspell_cast_success_rpc, JSON::print(info->to_dict()), cspell_cast_success, info);
}

void Entity::cspell_cast_success_rpc(String data) {
	Ref<SpellCastInfo> info;
	info.instance();
	info->from_dict(data_as_dict(data));
	info->resolve_references(this);

	cspell_cast_success(info);
}

void Entity::cspell_cast_success(Ref<SpellCastInfo> info) {
	con_spell_cast_success(info);
}

////    Cooldowns    ////
Vector<Ref<Cooldown> > *Entity::gets_cooldowns() {
	return &_s_cooldowns;
}
Vector<Ref<Cooldown> > *Entity::getc_cooldowns() {
	return &_c_cooldowns;
}

HashMap<int, Ref<Cooldown> > *Entity::gets_cooldown_map() {
	return &_s_cooldown_map;
}
HashMap<int, Ref<Cooldown> > *Entity::getc_cooldown_map() {
	return &_c_cooldown_map;
}

bool Entity::hass_cooldown(int spell_id) {
	return _s_cooldown_map.has(spell_id);
}
void Entity::adds_cooldown(int spell_id, float value) {
	if (_s_cooldown_map.has(spell_id)) {
		Ref<Cooldown> cd = _s_cooldown_map.get(spell_id);

		cd->set_remaining(value);

		son_cooldown_added(cd);

		emit_signal("scooldown_added", cd);

		ORPC(addc_cooldown, spell_id, value);

		return;
	}

	Ref<Cooldown> cd;
	cd.instance();

	cd->set_spell_id(spell_id);
	cd->set_remaining(value);

	_s_cooldown_map.set(spell_id, cd);
	_s_cooldowns.push_back(cd);

	son_cooldown_added(cd);

	emit_signal("scooldown_added", cd);

	ORPC(addc_cooldown, spell_id, value);
}
void Entity::removes_cooldown(int spell_id) {
	Ref<Cooldown> cd;

	if (_s_cooldown_map.has(spell_id)) {
		_s_cooldown_map.erase(spell_id);
	}

	for (int i = 0; i < _s_cooldowns.size(); ++i) {
		if (_s_cooldowns.get(i)->get_spell_id() == spell_id) {
			cd = _s_cooldowns.get(i);

			_s_cooldowns.remove(i);

			break;
		}
	}

	son_cooldown_removed(cd);

	emit_signal("scooldown_removed", cd);

	ORPC(removec_cooldown, spell_id);
}
Ref<Cooldown> Entity::gets_cooldown(int spell_id) {
	if (!_s_cooldown_map.has(spell_id)) {
		return Ref<Cooldown>();
	}

	return _s_cooldown_map.get(spell_id);
}
Ref<Cooldown> Entity::gets_cooldown_index(int index) {
	ERR_FAIL_INDEX_V(index, _s_cooldowns.size(), Ref<Cooldown>());

	return _s_cooldowns.get(index);
}
int Entity::gets_cooldown_count() {
	return _s_cooldowns.size();
}

bool Entity::hasc_cooldown(int spell_id) {
	return _c_cooldown_map.has(spell_id);
}
void Entity::addc_cooldown(int spell_id, float value) {
	if (_c_cooldown_map.has(spell_id)) {
		Ref<Cooldown> cd = _c_cooldown_map.get(spell_id);

		cd->set_remaining(value);

		con_cooldown_added(cd);

		emit_signal("ccooldown_added", cd);

		return;
	}

	Ref<Cooldown> cd;
	cd.instance();

	cd->set_spell_id(spell_id);
	cd->set_remaining(value);

	_c_cooldown_map.set(spell_id, cd);
	_c_cooldowns.push_back(cd);

	con_cooldown_added(cd);

	emit_signal("ccooldown_added", cd);
}
void Entity::removec_cooldown(int spell_id) {
	Ref<Cooldown> cd;

	if (_c_cooldown_map.has(spell_id)) {
		_c_cooldown_map.erase(spell_id);
	}

	for (int i = 0; i < _c_cooldowns.size(); ++i) {
		if (_c_cooldowns.get(i)->get_spell_id() == spell_id) {
			cd = _c_cooldowns.get(i);
			_c_cooldowns.remove(i);
			break;
		}
	}

	if (!cd.is_valid())
		cd.instance();

	con_cooldown_removed(cd);

	emit_signal("ccooldown_removed", cd);
}
Ref<Cooldown> Entity::getc_cooldown(int spell_id) {
	if (!_c_cooldown_map.has(spell_id)) {
		return Ref<Cooldown>();
	}

	return _c_cooldown_map.get(spell_id);
}
Ref<Cooldown> Entity::getc_cooldown_index(int index) {
	ERR_FAIL_INDEX_V(index, _c_cooldowns.size(), Ref<Cooldown>());

	return _c_cooldowns.get(index);
}
int Entity::getc_cooldown_count() {
	return _c_cooldowns.size();
}

//Category Cooldowns
Vector<Ref<CategoryCooldown> > Entity::gets_category_cooldowns() {
	return _s_category_cooldowns;
}
Vector<Ref<CategoryCooldown> > Entity::getc_category_cooldowns() {
	return _c_category_cooldowns;
}

bool Entity::hass_category_cooldown(int category_id) {
	return (category_id & _s_active_category_cooldowns) > 0;
}
void Entity::adds_category_cooldown(int category_id, float value) {
	if ((category_id & _s_active_category_cooldowns)) {
		for (int i = 0; i < _s_category_cooldowns.size(); ++i) {
			Ref<CategoryCooldown> cc = _s_category_cooldowns.get(i);

			if (cc->get_category_id() == category_id) {
				cc->set_remaining(value);

				son_category_cooldown_added(cc);

				emit_signal("scategory_cooldown_added", cc);

				ORPC(addc_category_cooldown, category_id, value);

				return;
			}
		}
	}

	Ref<CategoryCooldown> cc;
	cc.instance();

	cc->set_category_id(category_id);
	cc->set_remaining(value);

	_s_category_cooldowns.push_back(cc);

	_s_active_category_cooldowns |= category_id;

	son_category_cooldown_added(cc);

	emit_signal("scategory_cooldown_added", cc);

	ORPC(addc_category_cooldown, category_id, value);
}
void Entity::removes_category_cooldown(int category_id) {
	Ref<CategoryCooldown> cc;

	for (int i = 0; i < _s_category_cooldowns.size(); ++i) {
		if (_s_category_cooldowns.get(i)->get_category_id() == category_id) {
			cc = _s_category_cooldowns.get(i);
			_s_category_cooldowns.remove(i);
			break;
		}
	}

	if (!cc.is_valid())
		return;

	_s_active_category_cooldowns ^= category_id;

	son_category_cooldown_removed(cc);

	emit_signal("scategory_cooldown_removed", cc);

	ORPC(removec_category_cooldown, category_id);
}
Ref<CategoryCooldown> Entity::gets_category_cooldown(int category_id) {
	ERR_FAIL_COND_V(!(category_id & _s_active_category_cooldowns), Ref<CategoryCooldown>());

	Ref<CategoryCooldown> cc;

	for (int i = 0; i < _s_category_cooldowns.size(); ++i) {
		cc = _s_category_cooldowns.get(i);

		if (cc->get_category_id() == category_id) {
			return cc;
		}
	}

	return cc;
}
Ref<CategoryCooldown> Entity::gets_category_cooldown_index(int index) {
	ERR_FAIL_INDEX_V(index, _s_category_cooldowns.size(), Ref<Cooldown>());

	return _s_category_cooldowns.get(index);
}
int Entity::gets_category_cooldown_count() {
	return _s_category_cooldowns.size();
}

bool Entity::hasc_category_cooldown(int category_id) {
	return (category_id & _c_active_category_cooldowns) > 0;
}
void Entity::addc_category_cooldown(int category_id, float value) {
	if (category_id & _c_active_category_cooldowns) {
		for (int i = 0; i < _c_category_cooldowns.size(); ++i) {
			Ref<CategoryCooldown> cc = _c_category_cooldowns.get(i);

			if (cc->get_category_id() == category_id) {
				cc->set_remaining(value);

				con_category_cooldown_added(cc);

				emit_signal("ccategory_cooldown_added", cc);
				return;
			}
		}
	}

	Ref<CategoryCooldown> cc;
	cc.instance();

	cc->set_category_id(category_id);
	cc->set_remaining(value);

	_c_category_cooldowns.push_back(cc);

	_c_active_category_cooldowns |= category_id;

	con_category_cooldown_added(cc);

	emit_signal("ccategory_cooldown_added", cc);
}
void Entity::removec_category_cooldown(int category_id) {
	Ref<CategoryCooldown> cc;

	for (int i = 0; i < _c_category_cooldowns.size(); ++i) {
		if (_c_category_cooldowns.get(i)->get_category_id() == category_id) {
			cc = _c_category_cooldowns.get(i);

			_c_category_cooldowns.remove(i);

			break;
		}
	}

	if (!cc.is_valid())
		return;

	_c_active_category_cooldowns ^= category_id;

	con_category_cooldown_removed(cc);

	emit_signal("ccategory_cooldown_removed", cc);
}
Ref<CategoryCooldown> Entity::getc_category_cooldown(int category_id) {
	ERR_FAIL_COND_V(!(category_id & _c_active_category_cooldowns), Ref<CategoryCooldown>());

	Ref<CategoryCooldown> cc;

	for (int i = 0; i < _c_category_cooldowns.size(); ++i) {
		cc = _c_category_cooldowns.get(i);

		if (cc->get_category_id() == category_id) {
			return cc;
		}
	}

	return cc;
}
Ref<CategoryCooldown> Entity::getc_category_cooldown_index(int index) {
	ERR_FAIL_INDEX_V(index, _c_category_cooldowns.size(), Ref<Cooldown>());

	return _c_category_cooldowns.get(index);
}
int Entity::getc_category_cooldown_count() {
	return _c_category_cooldowns.size();
}

//Known Spells

int Entity::gets_free_spell_points() {
	return _s_free_spell_points;
}
void Entity::sets_free_spell_points(int value) {
	_s_free_spell_points = value;

	emit_signal("sfree_spell_points_changed", this, value);

	ORPC(setc_free_spell_points, value);
}

int Entity::getc_free_spell_points() {
	return _c_free_spell_points;
}
void Entity::setc_free_spell_points(int value) {
	_c_free_spell_points = value;

	emit_signal("cfree_spell_points_changed", this, value);
}

void Entity::crequest_spell_learn(int id) {
	slearn_spell(id);
}
void Entity::slearn_spell(int id) {
	if (has_method("_slearn_spell")) {
		call("_slearn_spell", id);
		return;
	}

	ERR_FAIL_COND(gets_free_spell_points() <= 0);

	ERR_FAIL_COND(!_s_entity_data.is_valid());

	Ref<EntityClassData> cd = _s_entity_data->get_entity_class_data();

	ERR_FAIL_COND(!cd.is_valid());

	for (int i = 0; i < cd->get_num_spells(); ++i) {
		Ref<Spell> sp = cd->get_spell(i);

		if (!sp.is_valid())
			continue;

		if (sp->get_id() == id) {
			Ref<Spell> req_spell = sp->get_training_required_spell();

			if (req_spell.is_valid() && !hass_spell(req_spell)) {
				return;
			}

			adds_spell(sp);
			sets_free_spell_points(_s_free_spell_points - 1);
			return;
		}
	}
}

bool Entity::hass_spell(Ref<Spell> spell) {
	for (int i = 0; i < _s_spells.size(); ++i) {
		if (_s_spells.get(i) == spell) {
			return true;
		}
	}

	return false;
}
bool Entity::hass_spell_id(int id) {
	for (int i = 0; i < _s_spells.size(); ++i) {
		Ref<Spell> spell = _s_spells.get(i);

		if (!spell.is_valid())
			continue;

		if (spell->get_id() == id) {
			return true;
		}
	}

	return false;
}
void Entity::adds_spell(Ref<Spell> spell) {
	if (hass_spell(spell))
		return;

	_s_spells.push_back(spell);

	emit_signal("sspell_added", this, spell);

	ORPCOBJ(addc_spell_rpc, spell->get_id(), addc_spell, spell);
}
void Entity::removes_spell(Ref<Spell> spell) {
	for (int i = 0; i < _s_spells.size(); ++i) {
		if (_s_spells.get(i) == spell) {
			_s_spells.remove(i);
			break;
		}
	}

	emit_signal("sspell_removed", this, spell);

	ORPCOBJ(removec_spell_rpc, spell->get_id(), removec_spell, spell);
}
Ref<Spell> Entity::gets_spell(int index) {
	ERR_FAIL_INDEX_V(index, _s_spells.size(), Ref<Spell>());

	return _s_spells.get(index);
}
int Entity::gets_spell_count() {
	return _s_spells.size();
}

bool Entity::hasc_spell(Ref<Spell> spell) {
	for (int i = 0; i < _c_spells.size(); ++i) {
		if (_c_spells.get(i) == spell) {
			return true;
		}
	}

	return false;
}
bool Entity::hasc_spell_id(int id) {
	for (int i = 0; i < _c_spells.size(); ++i) {
		Ref<Spell> spell = _c_spells.get(i);

		if (!spell.is_valid())
			continue;

		if (spell->get_id() == id) {
			return true;
		}
	}

	return false;
}
void Entity::addc_spell(Ref<Spell> spell) {
	if (hasc_spell(spell))
		return;

	_c_spells.push_back(spell);

	emit_signal("cspell_added", this, spell);
}
void Entity::removec_spell(Ref<Spell> spell) {
	for (int i = 0; i < _c_spells.size(); ++i) {
		if (_c_spells.get(i) == spell) {
			_c_spells.remove(i);
			break;
		}
	}

	emit_signal("cspell_removed", this, spell);
}
Ref<Spell> Entity::getc_spell(int index) {
	ERR_FAIL_INDEX_V(index, _c_spells.size(), Ref<Spell>());

	return _c_spells.get(index);
}
int Entity::getc_spell_count() {
	return _c_spells.size();
}

void Entity::addc_spell_rpc(int id) {
	ERR_FAIL_COND(EntityDataManager::get_instance() == NULL);

	addc_spell(EntityDataManager::get_instance()->get_spell(id));
}
void Entity::removec_spell_rpc(int id) {
	ERR_FAIL_COND(EntityDataManager::get_instance() == NULL);

	removec_spell(EntityDataManager::get_instance()->get_spell(id));
}

//Skills
bool Entity::hass_skill_id(int id) {
	for (int i = 0; i < _s_skills.size(); ++i) {
		Ref<EntitySkill> skill = _s_skills.get(i);

		ERR_CONTINUE(!skill.is_valid());

		if (skill->get_skill_id() == id) {
			return true;
		}
	}

	return false;
}
bool Entity::hass_skill(Ref<EntitySkill> skill) {
	for (int i = 0; i < _s_skills.size(); ++i) {
		if (_s_skills.get(i) == skill) {
			return true;
		}
	}

	return false;
}
void Entity::adds_skill(Ref<EntitySkill> skill) {
	if (hass_skill(skill))
		return;

	skill->connect("current_changed", this, "sskill_current_changed");
	skill->connect("max_changed", this, "sskill_max_changed");

	_s_skills.push_back(skill);

	emit_signal("sskill_added", this, skill);

	ORPC(addc_skill_id, skill->get_skill_id(), skill->get_current(), skill->get_max());
}
void Entity::removes_skill(Ref<EntitySkill> skill) {
	for (int i = 0; i < _s_skills.size(); ++i) {
		if (_s_skills.get(i) == skill) {
			_s_skills.remove(i);
			break;
		}
	}

	emit_signal("sskill_removed", this, skill);

	ORPC(removec_skill_id, skill->get_skill_id());
}
Ref<EntitySkill> Entity::gets_skill(int index) {
	ERR_FAIL_INDEX_V(index, _s_skills.size(), Ref<EntitySkill>());

	return _s_skills.get(index);
}
int Entity::gets_skill_count() {
	return _s_skills.size();
}

bool Entity::hasc_skill_id(int id) {
	for (int i = 0; i < _c_skills.size(); ++i) {
		Ref<EntitySkill> skill = _c_skills.get(i);

		ERR_CONTINUE(!skill.is_valid());

		if (skill->get_skill_id() == id) {
			return true;
		}
	}

	return false;
}
bool Entity::hasc_skill(Ref<EntitySkill> skill) {
	for (int i = 0; i < _c_skills.size(); ++i) {
		if (_c_skills.get(i) == skill) {
			return true;
		}
	}

	return false;
}
void Entity::addc_skill(Ref<EntitySkill> skill) {
	if (hasc_skill(skill))
		return;

	_c_skills.push_back(skill);

	emit_signal("cskill_added", this, skill);
}
void Entity::removec_skill(Ref<EntitySkill> skill) {
	for (int i = 0; i < _c_skills.size(); ++i) {
		if (_c_skills.get(i) == skill) {
			_c_skills.remove(i);
			break;
		}
	}

	emit_signal("cskill_removed", this, skill);
}
Ref<EntitySkill> Entity::getc_skill(int index) {
	ERR_FAIL_INDEX_V(index, _c_skills.size(), Ref<EntitySkill>());

	return _c_skills.get(index);
}
int Entity::getc_skill_count() {
	return _c_skills.size();
}

void Entity::sskill_current_changed(Ref<EntitySkill> skill) {
	//todo events

	ORPC(changec_skill, skill->get_skill_id(), skill->get_current());
}
void Entity::sskill_max_changed(Ref<EntitySkill> skill) {
	//todo events

	ORPC(changec_skill_max, skill->get_skill_id(), skill->get_max());
}

void Entity::addc_skill_id(int skill_id, int value, int max_value) {
	ERR_FAIL_COND(hasc_skill_id(skill_id));

	Ref<EntitySkill> skill;
	skill.instance();

	skill->set_skill_id(skill_id);
	skill->set_current(value);
	skill->set_max(max_value);

	addc_skill(skill);
}
void Entity::removec_skill_id(int skill_id) {
	for (int i = 0; i < _c_skills.size(); ++i) {
		Ref<EntitySkill> skill = _c_skills.get(i);

		ERR_CONTINUE(!skill.is_valid());

		if (skill->get_skill_id() == skill_id) {
			_c_skills.remove(i);

			emit_signal("cskill_removed", this, skill);

			return;
		}
	}
}
void Entity::changec_skill(int skill_id, int value) {
	for (int i = 0; i < _c_skills.size(); ++i) {
		Ref<EntitySkill> skill = _c_skills.get(i);

		ERR_CONTINUE(!skill.is_valid());

		if (skill->get_skill_id() == skill_id) {
			skill->set_current(value);

			emit_signal("cskill_changed", this, skill);

			return;
		}
	}
}
void Entity::changec_skill_max(int skill_id, int value) {
	for (int i = 0; i < _c_skills.size(); ++i) {
		Ref<EntitySkill> skill = _c_skills.get(i);

		ERR_CONTINUE(!skill.is_valid());

		if (skill->get_skill_id() == skill_id) {
			skill->set_max(value);

			emit_signal("cskill_changed", this, skill);

			return;
		}
	}
}

////    Casting System    ////

bool Entity::sis_casting() {
	return _s_spell_cast_info.is_valid();
}
bool Entity::cis_casting() {
	return _c_spell_cast_info.is_valid();
}

Ref<SpellCastInfo> Entity::gets_spell_cast_info() {
	return Ref<SpellCastInfo>(_s_spell_cast_info);
}

void Entity::sets_spell_cast_info(Ref<SpellCastInfo> info) {
	_s_spell_cast_info = Ref<SpellCastInfo>(info);
}

Ref<SpellCastInfo> Entity::getc_spell_cast_info() {
	return Ref<SpellCastInfo>(_c_spell_cast_info);
}

void Entity::setc_spell_cast_info(Ref<SpellCastInfo> info) {
	_c_spell_cast_info = Ref<SpellCastInfo>(info);
}

void Entity::sremove_auras_with_group(Ref<AuraGroup> aura_group) {
	for (int i = 0; i < _s_auras.size(); ++i) {
		Ref<AuraData> ad = _s_auras.get(i);

		if (ad->get_aura()->get_aura_group() == aura_group) {

			cremove_aura(ad);

			_s_auras.remove(i);

			emit_signal("saura_removed", ad);

			cremove_aura(ad);

			i--;
		}
	}
}

void Entity::crequest_target_change(NodePath path) {
	RPCS(net_sets_target, path);
}

void Entity::net_sets_target(NodePath path) {
	if (!ISSERVER())
		return;

	Node *p_target = get_node_or_null(path);

	sets_target(p_target);

	if (p_target == NULL) {
		VRPC(net_setc_target, NodePath());
	} else {
		if (gets_target() == NULL) {
			VRPC(net_setc_target, NodePath());
		} else {
			VRPC(net_setc_target, gets_target()->get_path());
		}
	}
}
void Entity::net_setc_target(NodePath path) {
	Node *p_target = get_node_or_null(path);

	setc_target(p_target);
}

Entity *Entity::gets_target() {
	return _s_target;
}

void Entity::sets_target(Node *p_target) {
	Entity *original_target = _s_target;

	if (!ObjectDB::instance_validate(original_target)) {
		original_target = NULL;
		_s_target = NULL;
	}

	if (p_target == NULL) {
		_s_target = NULL;

		if (has_method("_son_target_changed")) {
			call("_son_target_changed", this, original_target);
		}

		for (int i = 0; i < _s_resources.size(); ++i) {
			_s_resources.get(i)->ons_target_changed(this, original_target);
		}

		emit_signal("starget_changed", this, original_target);
		setc_target(p_target);
		return;
	}

	Entity *e = cast_to<Entity>(p_target);

	if (!e) {
		return;
	}

	_s_target = e;

	if (has_method("_son_target_changed")) {
		call("_son_target_changed", this, original_target);
	}

	for (int i = 0; i < _s_resources.size(); ++i) {
		_s_resources.get(i)->ons_target_changed(this, original_target);
	}

	emit_signal("starget_changed", this, original_target);

	if (is_inside_tree() && !get_tree()->has_network_peer()) {
		setc_target(p_target);
	}
}

Entity *Entity::getc_target() {
	return _c_target;
}
void Entity::setc_target(Node *p_target) {
	Entity *original_target = _c_target;

	if (!ObjectDB::instance_validate(original_target)) {
		original_target = NULL;
		_c_target = NULL;
	}

	if (p_target == NULL) {
		_c_target = NULL;

		if (has_method("_con_target_changed")) {
			call("_con_target_changed", this, original_target);
		}

		for (int i = 0; i < _c_resources.size(); ++i) {
			_c_resources.get(i)->onc_target_changed(this, original_target);
		}

		emit_signal("ctarget_changed", this, original_target);

		return;
	}

	Entity *e = cast_to<Entity>(p_target);

	if (!e) {
		return;
	}

	_c_target = e;

	if (has_method("_con_target_changed")) {
		call("_con_target_changed", this, original_target);
	}

	for (int i = 0; i < _c_resources.size(); ++i) {
		_c_resources.get(i)->onc_target_changed(this, original_target);
	}

	emit_signal("ctarget_changed", this, original_target);
}

////    Talents    ////

int Entity::gets_free_talent_points() {
	return _s_free_talent_points;
}
void Entity::sets_free_talent_points(int value) {
	_s_free_talent_points = value;

	ORPC(setc_free_talent_points, value);
}

int Entity::getc_free_talent_points() {
	return _c_free_talent_points;
}
void Entity::setc_free_talent_points(int value) {
	_c_free_talent_points = value;
}

void Entity::crequest_talent_learn(int spec_index, int talent_row, int talent_culomn) {
	sreceive_talent_learn_request(spec_index, talent_row, talent_culomn);
}

void Entity::sreceive_talent_learn_request(int spec_index, int talent_row, int talent_culomn) {
	call("_sreceive_talent_learn_request", spec_index, talent_row, talent_culomn);
}

void Entity::_sreceive_talent_learn_request(int spec_index, int talent_row, int talent_culomn) {
	if (gets_free_talent_points() <= 0)
		return;

	ERR_FAIL_COND(!_s_entity_data.is_valid());

	Ref<EntityClassData> class_data = _s_entity_data->get_entity_class_data();

	ERR_FAIL_COND(!class_data.is_valid());

	Ref<CharacterSpec> spec = class_data->get_spec(spec_index);

	ERR_FAIL_COND(!spec.is_valid());

	Ref<TalentRowData> tr = spec->get_talent_row(talent_row);

	ERR_FAIL_COND(!tr.is_valid());

	for (int i = 0; i < TalentRowData::MAX_TALENTS_PER_ENTRY; ++i) {
		Ref<Aura> talent = tr->get_talent(talent_culomn, i);

		if (!talent.is_valid())
			return;

		int talent_id = talent->get_id();

		if (hass_talent(talent_id))
			continue;

		if (talent->get_talent_required_talent().is_valid()) {
			if (!hass_talent(talent->get_talent_required_talent()->get_id())) {
				return;
			}
		}

		if (talent->get_talent_required_spell().is_valid()) {
			if (!hass_spell(talent->get_talent_required_spell())) {
				return;
			}
		}

		if (i > 0) {
			Ref<Aura> pt = tr->get_talent(talent_culomn, i - 1);

			for (int j = 0; j < sget_aura_count(); ++j) {
				Ref<AuraData> ad = sget_aura(j);

				if (ad->get_aura_id() == pt->get_id()) {
					sremove_aura(ad);

					break;
				}
			}
		}

		Ref<AuraApplyInfo> info;
		info.instance();

		info->set_caster(this);
		info->set_target(this);
		info->set_spell_scale(1);
		info->set_aura(talent);

		talent->sapply(info);

		adds_talent(talent_id);

		return;
	}
}

void Entity::crequest_talent_reset() {
	sreceive_reset_talent_request();
}
void Entity::sreceive_reset_talent_request() {
	call("_sreceive_reset_talent_request");
}
void Entity::_sreceive_reset_talent_request() {
	sreset_talents();
}

void Entity::sreset_talents() {

	_s_talents.clear();

	if (has_method("_son_talent_reset"))
		call("_son_talent_reset", this);

	emit_signal("stalent_reset", this);

	ORPC(creset_talents);
}
void Entity::creset_talents() {
	_c_talents.clear();

	if (has_method("_con_talent_reset"))
		call("_con_talent_reset", this);

	emit_signal("ctalent_reset", this);
}

void Entity::adds_talent(int talent) {
	if (hass_talent(talent))
		return;

	_s_talents.push_back(talent);

	if (has_method("_son_talent_learned"))
		call("_son_talent_learned", talent);

	emit_signal("stalent_learned", this, talent);

	ORPC(addc_talent, talent);
}
void Entity::removes_talent(int talent) {
	for (int i = 0; i < _s_talents.size(); ++i) {
		if (_s_talents[i] == talent) {
			_s_talents.remove(i);

			for (int j = 0; j < sget_aura_count(); ++j) {
				Ref<AuraData> ad = sget_aura(j);

				if (ad->get_aura_id() == talent) {
					sremove_aura(ad);

					break;
				}
			}

			break;
		}
	}

	ORPC(removec_talent, talent);
}
bool Entity::hass_talent(int talent) {
	for (int i = 0; i < _s_talents.size(); ++i) {
		if (_s_talents[i] == talent) {
			return true;
		}
	}

	return false;
}
int Entity::gets_talent(int index) {
	ERR_FAIL_INDEX_V(index, _s_talents.size(), 0);

	return _s_talents.get(index);
}
int Entity::gets_talent_count() {
	return _s_talents.size();
}
void Entity::sclear_talents() {
	_s_talents.clear();

	ORPC(cclear_talents);
}

void Entity::addc_talent(int talent) {
	if (hasc_talent(talent)) return;

	_c_talents.push_back(talent);

	if (has_method("_con_talent_learned"))
		call("_con_talent_learned", talent);

	emit_signal("ctalent_learned", this, talent);
}
void Entity::removec_talent(int talent) {
	for (int i = 0; i < _c_talents.size(); ++i) {
		if (_c_talents[i] == talent) {
			_c_talents.remove(i);
			return;
		}
	}
}
bool Entity::hasc_talent(int talent) {
	for (int i = 0; i < _c_talents.size(); ++i) {
		if (_c_talents[i] == talent) {
			return true;
		}
	}

	return false;
}
int Entity::getc_talent(int index) {
	ERR_FAIL_INDEX_V(index, _c_talents.size(), 0);

	return _c_talents.get(index);
}
int Entity::getc_talent_count() {
	return _c_talents.size();
}
void Entity::cclear_talents() {
	_c_talents.clear();
}

////    Bag    ////

Ref<Bag> Entity::gets_bag() const {
	return _s_bag;
}
void Entity::sets_bag(const Ref<Bag> bag) {
	if (_s_bag.is_valid()) {
		_s_bag->disconnect("item_added", this, "ons_item_added");
		_s_bag->disconnect("item_removed", this, "ons_item_removed");
		_s_bag->disconnect("item_swapped", this, "ons_items_swapped");
		_s_bag->disconnect("item_count_changed", this, "ons_item_count_changed");
		_s_bag->disconnect("overburdened", this, "ons_overburdened");
		_s_bag->disconnect("overburden_removed", this, "ons_overburden_removed");
	}

	_s_bag = bag;

	if (_s_bag.is_valid()) {
		_s_bag->connect("item_added", this, "ons_item_added");
		_s_bag->connect("item_removed", this, "ons_item_removed");
		_s_bag->connect("item_swapped", this, "ons_items_swapped");
		_s_bag->connect("item_count_changed", this, "ons_item_count_changed");
		_s_bag->connect("overburdened", this, "ons_overburdened");
		_s_bag->connect("overburden_removed", this, "ons_overburden_removed");
	}

	emit_signal("sbag_changed", this, _s_bag);

	if (_s_bag.is_valid()) {
		ORPC(setc_bag_rpc, JSON::print(_s_bag->to_dict()));
	} else {
		ORPC(setc_bag_rpc, "");
	}
}

Ref<Bag> Entity::getc_bag() const {
	return _c_bag;
}
void Entity::setc_bag(const Ref<Bag> bag) {
	_c_bag = bag;

	emit_signal("cbag_changed", this, _c_bag);
}

Ref<Bag> Entity::gets_target_bag() const {
	return _s_target_bag;
}
void Entity::sets_target_bag(const Ref<Bag> bag) {
	if (_s_target_bag.is_valid()) {
		_s_target_bag->disconnect("item_added", this, "ons_target_item_added");
		_s_target_bag->disconnect("item_removed", this, "ons_target_item_removed");
		_s_target_bag->disconnect("item_swapped", this, "ons_target_items_swapped");
		_s_target_bag->disconnect("item_count_changed", this, "ons_target_item_count_changed");
	}

	_s_target_bag = bag;

	if (_s_target_bag.is_valid()) {
		_s_target_bag->connect("item_added", this, "ons_target_item_added");
		_s_target_bag->connect("item_removed", this, "ons_target_item_removed");
		_s_target_bag->connect("item_swapped", this, "ons_target_items_swapped");
		_s_target_bag->connect("item_count_changed", this, "ons_target_item_count_changed");
	}

	emit_signal("starget_bag_changed", this, _s_target_bag);

	if (_s_target_bag.is_valid()) {
		ORPC(setc_target_bag_rpc, JSON::print(_s_target_bag->to_dict()));
	} else {
		ORPC(setc_target_bag_rpc, "");
	}
}

Ref<Bag> Entity::getc_target_bag() const {
	return _c_target_bag;
}
void Entity::setc_target_bag(const Ref<Bag> bag) {
	_c_target_bag = bag;

	emit_signal("ctarget_bag_changed", this, _s_target_bag);
}

void Entity::setc_bag_rpc(String data) {
	Ref<Bag> bag;
	bag.instance();
	bag->from_dict(data_as_dict(data));

	setc_bag(bag);
}
void Entity::setc_target_bag_rpc(String data) {
	Ref<Bag> bag;
	bag.instance();
	bag->from_dict(data_as_dict(data));

	setc_target_bag(bag);
}

void Entity::crequest_loot(int index) {
	RPCS(sloot, index);
}
void Entity::sloot(int index) {
	ERR_FAIL_COND(!_s_bag.is_valid());
	ERR_FAIL_COND(!_s_target_bag.is_valid());

	Ref<ItemInstance> ii = _s_target_bag->get_item(index);

	if (_s_bag->can_add_item(ii)) {
		_s_target_bag->remove_item(index);
		_s_bag->add_item(ii);
	}
}
void Entity::cloot(int index) {
	ERR_FAIL_COND(!_c_target_bag.is_valid());

	_c_target_bag->remove_item(index);
}

void Entity::ons_item_added(Ref<Bag> bag, Ref<ItemInstance> item, int slot_id) {
	ORPCOBJP(cadd_item_rpc, slot_id, JSON::print(item->to_dict()), cadd_item, slot_id, item);
}
void Entity::cadd_item_rpc(int slot_id, String item_data) {
	Ref<ItemInstance> ii;
	ii.instance();
	ii->from_dict(data_as_dict(item_data));

	cadd_item(slot_id, ii);
}
void Entity::cadd_item(int slot_id, Ref<ItemInstance> item) {
	ERR_FAIL_COND(!_c_bag.is_valid());

	_c_bag->add_item_at(slot_id, item);
}

void Entity::ons_item_removed(Ref<Bag> bag, Ref<ItemInstance> item, int slot_id) {
	ORPC(cremove_item, slot_id);
}
void Entity::sremove_item(const int slot_id) {
	ERR_FAIL_COND(!_s_bag.is_valid());

	_s_bag->remove_item(slot_id);
}
void Entity::cremove_item(const int slot_id) {
	ERR_FAIL_COND(!_c_bag.is_valid());

	_c_bag->remove_item(slot_id);
}
void Entity::cdenyremove_item(const int slot_id) {
}
void Entity::crequest_remove_item(const int slot_id) {
	RPCS(sremove_item, slot_id);
}

void Entity::ons_items_swapped(Ref<Bag> bag, int slot_id_1, int slot_id_2) {
	ORPC(cswap_items, slot_id_1, slot_id_2);
}
void Entity::sswap_items(int slot_id_1, int slot_id_2) {
	ERR_FAIL_COND(!_s_bag.is_valid());

	_s_bag->swap_items(slot_id_1, slot_id_2);
}
void Entity::cswap_items(int slot_id_1, int slot_id_2) {
	ERR_FAIL_COND(!_c_bag.is_valid());

	_c_bag->swap_items(slot_id_1, slot_id_2);
}
void Entity::cdeny_item_swap(int slot_id_1, int slot_id_2) {
}
void Entity::crequest_item_swap(int slot_id_1, int slot_id_2) {
	RPCS(sswap_items, slot_id_1, slot_id_2);
}

void Entity::ons_item_count_changed(Ref<Bag> bag, Ref<ItemInstance> item, int slot_id) {
	ERR_FAIL_COND(!item.is_valid());

	ORPC(cchange_item_count, slot_id, item->get_stack_size());
}
void Entity::cchange_item_count(int slot_id, int new_count) {
	ERR_FAIL_COND(!_c_bag.is_valid());

	_c_bag->set_item_count(slot_id, new_count);
}

void Entity::ons_overburdened(Ref<Bag> bag) {
}
void Entity::ons_overburden_removed(Ref<Bag> bag) {
}

//Target Bag

void Entity::ons_target_item_added(Ref<Bag> bag, Ref<ItemInstance> item, int slot_id) {
	ORPCOBJP(cadd_target_item_rpc, slot_id, JSON::print(item->to_dict()), cadd_target_item, slot_id, item);
}
void Entity::cadd_target_item_rpc(int slot_id, String item_data) {
	Ref<ItemInstance> ii;
	ii.instance();
	ii->from_dict(data_as_dict(item_data));

	cadd_target_item(slot_id, ii);
}
void Entity::cadd_target_item(int slot_id, Ref<ItemInstance> item) {
	ERR_FAIL_COND(!_c_target_bag.is_valid());

	_c_target_bag->add_item_at(slot_id, item);
}

void Entity::ons_target_item_removed(Ref<Bag> bag, Ref<ItemInstance> item, int slot_id) {
	ORPC(cremove_target_item, slot_id);
}
void Entity::sremove_target_item(const int slot_id) {
	ERR_FAIL_COND(!_s_target_bag.is_valid());

	_s_target_bag->remove_item(slot_id);
}
void Entity::cremove_target_item(const int slot_id) {
	ERR_FAIL_COND(!_c_target_bag.is_valid());

	_c_target_bag->remove_item(slot_id);
}
void Entity::cdenyremove_target_item(const int slot_id) {
}
void Entity::crequest_target_remove_item(const int slot_id) {
	RPCS(sremove_target_item, slot_id);
}

void Entity::ons_target_items_swapped(Ref<Bag> bag, int slot_id_1, int slot_id_2) {
	ORPC(cswap_target_items, slot_id_1, slot_id_2);
}
void Entity::sswap_target_items(int slot_id_1, int slot_id_2) {
	ERR_FAIL_COND(!_s_target_bag.is_valid());

	_s_target_bag->swap_items(slot_id_1, slot_id_2);
}
void Entity::cswap_target_items(int slot_id_1, int slot_id_2) {
	ERR_FAIL_COND(!_c_target_bag.is_valid());

	_c_target_bag->swap_items(slot_id_1, slot_id_2);
}
void Entity::cdeny_target_item_swap(int slot_id_1, int slot_id_2) {
}
void Entity::crequest_target_item_swap(int slot_id_1, int slot_id_2) {
	RPCS(sswap_target_items, slot_id_1, slot_id_2);
}

void Entity::ons_target_item_count_changed(Ref<Bag> bag, Ref<ItemInstance> item, int slot_id) {
	ERR_FAIL_COND(!item.is_valid());

	ORPC(cchange_target_item_count, slot_id, item->get_stack_size());
}
void Entity::cchange_target_item_count(int slot_id, int new_count) {
	ERR_FAIL_COND(!_c_target_bag.is_valid());

	Ref<ItemInstance> ii = _c_target_bag->get_item(slot_id);

	ERR_FAIL_COND(!ii.is_valid());

	ii->set_stack_size(new_count);
}

////    DATA    ////
void Entity::adds_data(Ref<EntityDataContainer> data) {
	_s_data.push_back(data);
}
void Entity::removes_data(int index) {
	ERR_FAIL_INDEX(index, _s_data.size());

	_s_data.remove(index);
}
Ref<EntityDataContainer> Entity::gets_data(int index) {
	ERR_FAIL_INDEX_V(index, _s_data.size(), Ref<EntityDataContainer>());

	return _s_data.get(index);
}
int Entity::gets_data_count() {
	return _s_data.size();
}

void Entity::addc_data(Ref<EntityDataContainer> data) {
	_c_data.push_back(data);
}
void Entity::removec_data(int index) {
	ERR_FAIL_INDEX(index, _c_data.size());

	_c_data.remove(index);
}
Ref<EntityDataContainer> Entity::getc_data(int index) {
	ERR_FAIL_INDEX_V(index, _c_data.size(), Ref<EntityDataContainer>());

	return _c_data.get(index);
}
int Entity::getc_data_count() {
	return _c_data.size();
}

////    Actionbars    ////

bool Entity::get_actionbar_locked() {
	return _actionbar_locked;
}
void Entity::set_actionbar_locked(bool value) {
	_actionbar_locked = value;
}

Ref<ActionBarProfile> Entity::get_action_bar_profile() {
	return _action_bar_profile;
}

void Entity::loaded() {
	//sendstate = true;
}

////    PlayerData    ////

bool Entity::get_maunal_process() const {
	return _maunal_process;
}
void Entity::set_maunal_process(const bool value) {
	_maunal_process = value;
}

void Entity::update(float delta) {
	if (_s_gcd > 0.0000001) {
		_s_gcd -= delta;

		if (_s_gcd <= 0) {
			_s_gcd = 0;

			void son_gcd_finished();

			emit_signal("sgcd_finished");
		}
	}

	if (_c_gcd > 0.0000001) {
		_c_gcd -= delta;

		if (_c_gcd <= 0) {
			_c_gcd = 0;

			void con_gcd_finished();

			emit_signal("cgcd_finished");
		}
	}

	for (int i = 0; i < _c_cooldowns.size(); ++i) {
		Ref<Cooldown> cd = _c_cooldowns.get(i);

		cd->update(delta);
	}

	for (int i = 0; i < _c_category_cooldowns.size(); ++i) {
		Ref<CategoryCooldown> cd = _c_category_cooldowns.get(i);

		cd->update(delta);
	}

	for (int i = 0; i < _s_cooldowns.size(); ++i) {
		Ref<Cooldown> cd = _s_cooldowns.get(i);

		if (cd->update(delta)) {
			removes_cooldown(cd->get_spell_id());
			--i;
		}
	}

	for (int i = 0; i < _s_category_cooldowns.size(); ++i) {
		Ref<CategoryCooldown> cd = _s_category_cooldowns.get(i);

		if (cd->update(delta)) {
			removes_category_cooldown(cd->get_category_id());
			--i;
		}
	}

	update_auras(delta);

	if (ISSERVER()) {
		if (_s_spell_cast_info.is_valid() && _s_spell_cast_info->get_is_casting()) {
			if (_s_spell_cast_info->update_cast_time(delta)) {
				sfinish_cast();
			}
		}

		for (int i = 0; i < Stat::STAT_ID_TOTAL_STATS; ++i) {
			Ref<Stat> s = _stats[i];

			if (s->get_dirty_mods())
				s->apply_modifiers();

			if (s->get_dirty()) {
				ssend_stat(s->get_id(), s->gets_current(), s->gets_max());

				s->set_dirty(false);
			}
		}

		if (_s_entity_controller == EntityEnums::ENITIY_CONTROLLER_AI && _s_ai->get_enabled()) {
			if (_s_pet_owner)
				_s_ai->pet_update(delta);
			else
				_s_ai->update(delta);
		}

		for (int i = 0; i < _s_resources.size(); ++i) {
			Ref<EntityResource> res = _s_resources.get(i);

			ERR_CONTINUE(!res.is_valid());

			if (res->get_should_process())
				res->process_server(delta);
		}
	}

	if (ISCLIENT()) {
		if (_c_spell_cast_info.is_valid() && _c_spell_cast_info->get_is_casting()) {
			_c_spell_cast_info->update_cast_time(delta);
		}

		for (int i = 0; i < _c_resources.size(); ++i) {
			Ref<EntityResource> res = _c_resources.get(i);

			ERR_CONTINUE(!res.is_valid());

			if (res->get_should_process())
				res->process_client(delta);
		}
	}
}

String Entity::random_name() {
	return "";
}

//Networking

Entity *Entity::gets_sees(int index) {
	ERR_FAIL_INDEX_V(index, _s_sees.size(), NULL);

	return _s_sees.get(index);
}
void Entity::removes_sees_index(int index) {
	Entity *e = _s_sees.get(index);

	if (unlikely(!ObjectDB::instance_validate(e))) {
		_s_sees.remove(index);
		return;
	}

	e->removes_seen_by(this);

	_s_sees.remove(index);
}
void Entity::removes_sees(Entity *entity) {
	if (unlikely(!ObjectDB::instance_validate(entity))) {
		_s_sees.erase(entity);
		return;
	}

	entity->removes_seen_by(this);

	_s_sees.erase(entity);
}
void Entity::removes_sees_bind(Node *entity) {
	Entity *e = Object::cast_to<Entity>(entity);

	ERR_FAIL_COND(!e);

	removes_sees(e);
}
void Entity::adds_sees(Entity *entity) {
	ERR_FAIL_COND(!ObjectDB::instance_validate(entity));

	entity->adds_seen_by(this);

	for (int i = 0; i < _s_sees.size(); ++i) {
		if (_s_sees.get(i) == entity)
			return;
	}

	_s_sees.push_back(entity);
}
void Entity::adds_sees_bind(Node *entity) {
	Entity *e = Object::cast_to<Entity>(entity);

	ERR_FAIL_COND(!e);

	adds_sees(e);
}
int Entity::gets_sees_count() {
	return _s_sees.size();
}

Entity *Entity::gets_seen_by(int index) {
	ERR_FAIL_INDEX_V(index, _s_seen_by.size(), NULL);

	return _s_seen_by.get(index);
}
void Entity::removes_seen_by_index(int index) {
	_s_seen_by.remove(index);
}
void Entity::removes_seen_by(Entity *entity) {
	_s_seen_by.erase(entity);
}
void Entity::removes_seen_by_bind(Node *entity) {
	Entity *e = Object::cast_to<Entity>(entity);

	ERR_FAIL_COND(!e);

	removes_seen_by(e);
}
void Entity::adds_seen_by(Entity *entity) {
	ERR_FAIL_COND(!ObjectDB::instance_validate(entity));

	for (int i = 0; i < _s_seen_by.size(); ++i) {
		if (_s_seen_by.get(i) == entity)
			return;
	}

	_s_seen_by.push_back(entity);
}
void Entity::adds_seen_by_bind(Node *entity) {
	Entity *e = Object::cast_to<Entity>(entity);

	ERR_FAIL_COND(!e);

	adds_seen_by(e);
}

int Entity::gets_seen_by_count() {
	return _s_seen_by.size();
}

void Entity::vrpc(const StringName &p_method, VARIANT_ARG_DECLARE) {

	VARIANT_ARGPTRS;

	int argc = 0;
	for (int i = 0; i < VARIANT_ARG_MAX; i++) {
		if (argptr[i]->get_type() == Variant::NIL)
			break;
		argc++;
	}

	for (int i = 0; i < _s_seen_by.size(); ++i) {
		Entity *e = _s_seen_by.get(i);

		if (unlikely(!ObjectDB::instance_validate(e))) {
			_s_seen_by.remove(i);
			--i;
			continue;
		}

		int netm = e->get_network_master();

		if (netm != 1)
			rpcp(netm, false, p_method, argptr, argc);
	}

	if (get_network_master() != 1)
		rpcp(get_network_master(), false, p_method, argptr, argc);
}

Variant Entity::_vrpc_bind(const Variant **p_args, int p_argcount, Variant::CallError &r_error) {

	if (p_argcount < 1) {
		r_error.error = Variant::CallError::CALL_ERROR_TOO_FEW_ARGUMENTS;
		r_error.argument = 1;
		return Variant();
	}

	if (p_args[0]->get_type() != Variant::STRING) {
		r_error.error = Variant::CallError::CALL_ERROR_INVALID_ARGUMENT;
		r_error.argument = 0;
		r_error.expected = Variant::STRING;
		return Variant();
	}

	StringName method = *p_args[0];

	for (int i = 0; i < _s_seen_by.size(); ++i) {
		Entity *e = _s_seen_by.get(i);

		if (unlikely(!ObjectDB::instance_validate(e))) {
			_s_seen_by.remove(i);
			--i;
			continue;
		}

		int netm = e->get_network_master();

		if (netm != 1)
			rpcp(netm, false, method, &p_args[1], p_argcount - 1);
	}

	//call(method, &p_args[1], p_argcount - 1);

	r_error.error = Variant::CallError::CALL_OK;
	return Variant();
}

Dictionary Entity::data_as_dict(String &data) {
	Error err;
	String err_txt;
	int err_line;
	Variant v;
	err = JSON::parse(data, v, err_txt, err_line);

	ERR_FAIL_COND_V(err != OK, v);

	Dictionary d = v;

	return d;
}

void Entity::register_for_physics_process(Ref<SpellCastInfo> info) {
	_physics_process_scis.push_back(info);
}

Entity::Entity() {
	_maunal_process = false;
	_deserialized = false;

	_s_guid = 0;
	_c_guid = 0;

	_s_class_id = 0;
	_c_class_id = 0;

	_s_type = 0;
	_c_type = 0;

	_s_gender = EntityEnums::GENDER_MALE;
	_c_gender = EntityEnums::GENDER_MALE;

	_s_level = 1;
	_c_level = 1;

	_s_xp = 0;
	_c_xp = 0;

	_s_send_flag = 0;

	_c_money = 0;
	_s_money = 0;

	_s_entity_name = "";
	_c_entity_name = "";

	_s_state = PlayerStates::STATE_NORMAL;
	_c_state = PlayerStates::STATE_NORMAL;

	_s_gcd = 0;
	_c_gcd = 0;

	_s_is_dead = 0;
	_c_is_dead = 0;

	_s_interaction_type = EntityEnums::ENITIY_INTERACTION_TYPE_NORMAL;
	_c_interaction_type = EntityEnums::ENITIY_INTERACTION_TYPE_NORMAL;

	_s_seed = 0;
	_c_seed = _s_seed;

	for (int i = 0; i < EntityEnums::ENTITY_STATE_TYPE_INDEX_MAX; ++i) {
		_s_states[i] = 0;
	}

	_s_state = 0;
	_c_state = 0;

	_s_active_category_cooldowns = 0;
	_c_active_category_cooldowns = 0;

	_s_entity_type = EntityEnums::ENITIY_TYPE_NONE;
	_c_entity_type = EntityEnums::ENITIY_TYPE_NONE;

	_s_immunity_flags = 0;

	_s_entity_flags = 0;
	_c_entity_flags = 0;

	_s_target = NULL;
	_c_target = NULL;

	_s_free_talent_points = 0;
	_c_free_talent_points = 0;

	_s_free_spell_points = 0;
	_c_free_spell_points = 0;

	_action_bar_profile.instance();
	_actionbar_locked = false;

	for (int i = 0; i < Stat::STAT_ID_TOTAL_STATS; ++i) {
		Ref<Stat> s = Ref<Stat>(memnew(Stat(static_cast<Stat::StatId>(i), this)));

		_stats[i] = s;
	}

	_sai_state = EntityEnums::AI_STATE_OFF;
	_sai_state_stored = EntityEnums::AI_STATE_OFF;

	_s_original_entity_controller = EntityEnums::ENITIY_CONTROLLER_NONE;
	_s_entity_controller = EntityEnums::ENITIY_CONTROLLER_NONE;

	_s_pet_owner = NULL;
	_c_pet_owner = NULL;
	_s_pet_formation_index = 0;
	_s_pet_ai_state = EntityEnums::AI_STATE_OFF;

	SET_RPC_REMOTE("csend_request_rank_increase");
	SET_RPC_REMOTE("csend_request_rank_decrease");

	SET_RPC_REMOTE("setc_guid");
	SET_RPC_REMOTE("setc_entity_data_id");
	SET_RPC_REMOTE("setc_entity_type");
	SET_RPC_REMOTE("setc_entity_name");
	SET_RPC_REMOTE("setc_gender");
	SET_RPC_REMOTE("setc_level");
	SET_RPC_REMOTE("setc_xp");
	SET_RPC_REMOTE("setc_seed");

	//EntityType

	SET_RPC_REMOTE("setc_entity_type");

	//EntityInteractionType

	SET_RPC_REMOTE("setc_entity_interaction_type");
	SET_RPC_REMOTE("setc_entity_flags");
	SET_RPC_REMOTE("setc_entity_name");
	SET_RPC_REMOTE("setc_gender");
	SET_RPC_REMOTE("setc_level");
	SET_RPC_REMOTE("setc_xp");
	SET_RPC_REMOTE("setc_money");
	SET_RPC_REMOTE("setc_entity_data_id");

	////     Stats    ////

	SET_RPC_REMOTE("creceive_stat");

	SET_RPC_REMOTE("cdie");

	//send_stat

	////    Equip Slots    ////

	SET_RPC_REMOTE("sequip");
	SET_RPC_REMOTE("cequip_success");
	SET_RPC_REMOTE("cequip_fail");

	////    Resources    ////

	SET_RPC_REMOTE("addc_resource_rpc");
	SET_RPC_REMOTE("removec_resource");
	SET_RPC_REMOTE("clearc_resource");

	SET_RPC_REMOTE("creceive_resource_current");
	SET_RPC_REMOTE("creceive_resource_curr_max");
	SET_RPC_REMOTE("creceive_resource_data");

	////    Global Cooldown    ////

	SET_RPC_REMOTE("cstart_global_cooldown");

	////    States    ////

	SET_RPC_REMOTE("setc_state");

	////    Crafting System    ////

	SET_RPC_REMOTE("scraft");

	SET_RPC_REMOTE("addc_craft_recipe_id");
	SET_RPC_REMOTE("removec_craft_recipe_id");

	////    SpellSystem    ////

	SET_RPC_REMOTE("scast_spell");

	//Damage Operations

	SET_RPC_REMOTE("stake_damage");
	SET_RPC_REMOTE("stake_damage");

	//Heal Operations

	SET_RPC_REMOTE("stake_heal");
	SET_RPC_REMOTE("sdeal_heal_to");

	//Damage, Heal RPCs

	SET_RPC_REMOTE("cdamage_dealt_rpc");
	SET_RPC_REMOTE("cdealt_damage_rpc");
	SET_RPC_REMOTE("cheal_dealt_rpc");
	SET_RPC_REMOTE("cdealt_heal_rpc");

	//Interactions

	SET_RPC_REMOTE("sinteract");

	SET_RPC_REMOTE("copen_window");

	//XP Operations

	SET_RPC_REMOTE("addc_xp");
	SET_RPC_REMOTE("clevelup");

	//Aura Manipulation

	SET_RPC_REMOTE("cadd_aura_rpc");
	SET_RPC_REMOTE("cremove_aura_rpc");
	SET_RPC_REMOTE("cremove_aura_exact_rpc");
	SET_RPC_REMOTE("cremove_aura_expired_rpc");
	SET_RPC_REMOTE("cremove_aura_dispelled_rpc");
	SET_RPC_REMOTE("caura_refreshed_rpc");

	////    Casting System    ////

	SET_RPC_REMOTE("setc_spell_cast_info");

	SET_RPC_REMOTE("cstart_casting_rpc");
	SET_RPC_REMOTE("cfail_cast");
	SET_RPC_REMOTE("cdelay_cast");
	SET_RPC_REMOTE("cfinish_cast");
	SET_RPC_REMOTE("cinterrupt_cast");

	SET_RPC_REMOTE("cspell_cast_success_rpc");

	////    Cooldowns    ////

	SET_RPC_REMOTE("addc_cooldown");
	SET_RPC_REMOTE("removec_cooldown");

	//Category Cooldowns

	SET_RPC_REMOTE("addc_category_cooldown");
	SET_RPC_REMOTE("removec_category_cooldown");

	//Known Spells

	SET_RPC_REMOTE("setc_free_spell_points");
	SET_RPC_REMOTE("slearn_spell");

	SET_RPC_REMOTE("addc_spell_rpc");
	SET_RPC_REMOTE("removec_spell_rpc");

	//Skills

	SET_RPC_REMOTE("addc_skill_id");
	SET_RPC_REMOTE("removec_skill_id");
	SET_RPC_REMOTE("changec_skill");
	SET_RPC_REMOTE("changec_skill_max");

	////    Target    ////

	SET_RPC_REMOTE("net_sets_target");
	SET_RPC_REMOTE("net_setc_target");

	////    Talents    ////

	SET_RPC_REMOTE("setc_free_talent_points");
	SET_RPC_REMOTE("sreceive_talent_learn_request");
	SET_RPC_REMOTE("sreceive_reset_talent_request");

	SET_RPC_REMOTE("sreset_talents");

	SET_RPC_REMOTE("addc_talent");
	SET_RPC_REMOTE("removec_talent");

	////    Inventory    ////

	SET_RPC_REMOTE("setc_bag_rpc");
	SET_RPC_REMOTE("setc_target_bag_rpc");

	SET_RPC_REMOTE("sloot");
	SET_RPC_REMOTE("cloot");

	SET_RPC_REMOTE("cadd_item_rpc");
	SET_RPC_REMOTE("sremove_item");
	SET_RPC_REMOTE("cremove_item");
	SET_RPC_REMOTE("cdenyremove_item");
	SET_RPC_REMOTE("sswap_items");
	SET_RPC_REMOTE("cswap_items");
	SET_RPC_REMOTE("cdeny_item_swap");
	SET_RPC_REMOTE("cchange_item_count");

	SET_RPC_REMOTE("cadd_target_item_rpc");
	SET_RPC_REMOTE("sremove_target_item");
	SET_RPC_REMOTE("cremove_target_item");
	SET_RPC_REMOTE("cdenyremove_target_item");
	SET_RPC_REMOTE("sswap_target_items");
	SET_RPC_REMOTE("cswap_target_items");
	SET_RPC_REMOTE("cdeny_target_item_swap");
	SET_RPC_REMOTE("cchange_target_item_count");

	////    Data    ////

	SET_RPC_REMOTE("addc_data");
	SET_RPC_REMOTE("removec_data");
}

Entity::~Entity() {
	//Ref<EntityData> _s_entity_data;
	//Ref<EntityData> _c_entity_data;

	_s_resources.clear();
	_c_resources.clear();

	//Ref<SpellCastInfo> _s_spell_cast_info;
	//Ref<SpellCastInfo> _c_spell_cast_info;

	_s_auras.clear();
	_c_auras.clear();

	_s_cooldowns.clear();
	_c_cooldowns.clear();

	_s_cooldown_map.clear();
	_c_cooldown_map.clear();

	_s_category_cooldowns.clear();
	_c_category_cooldowns.clear();

	_s_data.clear();
	_c_data.clear();

	_s_craft_recipes.clear();
	_c_craft_recipes.clear();

	_s_spells.clear();
	_c_spells.clear();

	_s_free_talent_points = 0;
	_c_free_talent_points = 0;

	_s_talents.clear();
	_c_talents.clear();

	for (int i = 0; i < Stat::STAT_ID_TOTAL_STATS; ++i) {
		_stats[i].unref();
	}

	for (int i = 0; i < ItemEnums::EQUIP_SLOT_EQUIP_SLOT_MAX; ++i) {
		_s_equipment[i].unref();
		_c_equipment[i].unref();
	}

	_action_bar_profile.unref();

	_s_sees.clear();
	_s_seen_by.clear();

	_s_ai.unref();

	_s_pets.clear();

	_physics_process_scis.clear();
}

void Entity::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_INSTANCED: {
			_body = get_node_or_null(_body_path);

			if (ObjectDB::instance_validate(_body))
				_body->set_owner(this);

			_character_skeleton = get_node_or_null(_character_skeleton_path);

			if (_character_skeleton != NULL) {
				if (_character_skeleton->has_method("set_gender"))
					_character_skeleton->call("set_gender", _c_gender);
			}
		}
		case NOTIFICATION_ENTER_TREE: {
			if (!Engine::get_singleton()->is_editor_hint())
				set_process(true);

			if (!_body) {
				_body = get_node_or_null(_body_path);

				if (ObjectDB::instance_validate(_body))
					_body->set_owner(this);
			}

			if (!_character_skeleton) {
				_character_skeleton = get_node_or_null(_character_skeleton_path);

				if (_character_skeleton != NULL) {
					if (_character_skeleton->has_method("set_gender"))
						_character_skeleton->call("set_gender", _c_gender);
				}
			}
		} break;
		case NOTIFICATION_PROCESS: {
			if (!_maunal_process)
				update(get_process_delta_time());
		} break;
		case NOTIFICATION_PHYSICS_PROCESS: {
			son_physics_process(get_physics_process_delta_time());
		} break;
		case NOTIFICATION_EXIT_TREE: {
			for (int i = 0; i < _s_seen_by.size(); ++i) {
				Entity *e = _s_seen_by.get(i);

				if (ObjectDB::instance_validate(e)) {
					e->removes_sees(this);
				}
			}
		} break;
	}
}

void Entity::_bind_methods() {
	//Signals
	ADD_SIGNAL(MethodInfo("deserialized", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity")));

	ADD_SIGNAL(MethodInfo("sname_changed", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity")));
	ADD_SIGNAL(MethodInfo("cname_changed", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity")));

	ADD_SIGNAL(MethodInfo("starget_changed", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "old_target", PROPERTY_HINT_RESOURCE_TYPE, "Entity")));
	ADD_SIGNAL(MethodInfo("ctarget_changed", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "old_target", PROPERTY_HINT_RESOURCE_TYPE, "Entity")));

	ADD_SIGNAL(MethodInfo("son_damage_received", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellDamageInfo")));
	ADD_SIGNAL(MethodInfo("con_damage_received", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellDamageInfo")));

	ADD_SIGNAL(MethodInfo("con_damage_dealt", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellDamageInfo")));
	ADD_SIGNAL(MethodInfo("con_dealt_damage", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellDamageInfo")));

	ADD_SIGNAL(MethodInfo("son_heal_received", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellHealInfo")));
	ADD_SIGNAL(MethodInfo("con_heal_received", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellHealInfo")));

	ADD_SIGNAL(MethodInfo("son_dealt_heal", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellHealInfo")));
	ADD_SIGNAL(MethodInfo("con_dealt_heal", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellHealInfo")));

	ADD_SIGNAL(MethodInfo("son_heal_dealt", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellHealInfo")));
	ADD_SIGNAL(MethodInfo("con_heal_dealt", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellHealInfo")));

	ADD_SIGNAL(MethodInfo("sentity_data_changed", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "EntityData")));
	ADD_SIGNAL(MethodInfo("centity_data_changed", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "EntityData")));

	ADD_SIGNAL(MethodInfo("sdied", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity")));
	ADD_SIGNAL(MethodInfo("cdied", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity")));

	//SpellCastSignals
	ADD_SIGNAL(MethodInfo("scast_started", PropertyInfo(Variant::OBJECT, "spell_cast_info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	ADD_SIGNAL(MethodInfo("scast_failed", PropertyInfo(Variant::OBJECT, "spell_cast_info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	ADD_SIGNAL(MethodInfo("scast_delayed", PropertyInfo(Variant::OBJECT, "spell_cast_info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	ADD_SIGNAL(MethodInfo("scast_finished", PropertyInfo(Variant::OBJECT, "spell_cast_info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	ADD_SIGNAL(MethodInfo("scast_interrupted", PropertyInfo(Variant::OBJECT, "spell_cast_info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	ADD_SIGNAL(MethodInfo("sspell_cast_success", PropertyInfo(Variant::OBJECT, "spell_cast_info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));

	ADD_SIGNAL(MethodInfo("ccast_started", PropertyInfo(Variant::OBJECT, "spell_cast_info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	ADD_SIGNAL(MethodInfo("ccast_failed", PropertyInfo(Variant::OBJECT, "spell_cast_info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	ADD_SIGNAL(MethodInfo("ccast_delayed", PropertyInfo(Variant::OBJECT, "spell_cast_info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	ADD_SIGNAL(MethodInfo("ccast_finished", PropertyInfo(Variant::OBJECT, "spell_cast_info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	ADD_SIGNAL(MethodInfo("ccast_interrupted", PropertyInfo(Variant::OBJECT, "spell_cast_info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	ADD_SIGNAL(MethodInfo("cspell_cast_success", PropertyInfo(Variant::OBJECT, "spell_cast_info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));

	//Aura signals
	ADD_SIGNAL(MethodInfo("saura_added", PropertyInfo(Variant::OBJECT, "aura_data", PROPERTY_HINT_RESOURCE_TYPE, "AuraData")));
	ADD_SIGNAL(MethodInfo("saura_removed", PropertyInfo(Variant::OBJECT, "aura_data", PROPERTY_HINT_RESOURCE_TYPE, "AuraData")));
	ADD_SIGNAL(MethodInfo("saura_removed_expired", PropertyInfo(Variant::OBJECT, "aura_data", PROPERTY_HINT_RESOURCE_TYPE, "AuraData")));
	ADD_SIGNAL(MethodInfo("saura_removed_dispelled", PropertyInfo(Variant::OBJECT, "aura_data", PROPERTY_HINT_RESOURCE_TYPE, "AuraData")));
	//ADD_SIGNAL(MethodInfo("saura_refreshed", PropertyInfo(Variant::OBJECT, "aura_data", PROPERTY_HINT_RESOURCE_TYPE, "AuraData")));

	ADD_SIGNAL(MethodInfo("caura_added", PropertyInfo(Variant::OBJECT, "aura_data", PROPERTY_HINT_RESOURCE_TYPE, "AuraData")));
	ADD_SIGNAL(MethodInfo("caura_removed", PropertyInfo(Variant::OBJECT, "aura_data", PROPERTY_HINT_RESOURCE_TYPE, "AuraData")));
	ADD_SIGNAL(MethodInfo("caura_removed_dispelled", PropertyInfo(Variant::OBJECT, "aura_data", PROPERTY_HINT_RESOURCE_TYPE, "AuraData")));
	ADD_SIGNAL(MethodInfo("caura_removed_expired", PropertyInfo(Variant::OBJECT, "aura_data", PROPERTY_HINT_RESOURCE_TYPE, "AuraData")));
	//ADD_SIGNAL(MethodInfo("caura_refreshed", PropertyInfo(Variant::OBJECT, "aura_data", PROPERTY_HINT_RESOURCE_TYPE, "AuraData")));

	//Skills
	ADD_SIGNAL(MethodInfo("sskill_added", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "skill", PROPERTY_HINT_RESOURCE_TYPE, "EntitySkill")));
	ADD_SIGNAL(MethodInfo("sskill_removed", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "skill", PROPERTY_HINT_RESOURCE_TYPE, "EntitySkill")));

	ADD_SIGNAL(MethodInfo("cskill_added", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "skill", PROPERTY_HINT_RESOURCE_TYPE, "EntitySkill")));
	ADD_SIGNAL(MethodInfo("cskill_removed", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "skill", PROPERTY_HINT_RESOURCE_TYPE, "EntitySkill")));
	ADD_SIGNAL(MethodInfo("cskill_changed", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "skill", PROPERTY_HINT_RESOURCE_TYPE, "EntitySkill")));

	//setup
	BIND_VMETHOD(MethodInfo("_setup"));

	//Windows
	ADD_SIGNAL(MethodInfo("onc_open_loot_winow_request"));
	ADD_SIGNAL(MethodInfo("onc_open_container_winow_request"));
	ADD_SIGNAL(MethodInfo("onc_open_vendor_winow_request"));

	ClassDB::bind_method(D_METHOD("setup"), &Entity::setup);
	ClassDB::bind_method(D_METHOD("_setup"), &Entity::_setup);
	ClassDB::bind_method(D_METHOD("setup_actionbars"), &Entity::setup_actionbars);

	//binds

	ClassDB::bind_method(D_METHOD("ssend_stat", "id", "ccurrent", "cmax"), &Entity::ssend_stat);
	ClassDB::bind_method(D_METHOD("creceive_stat", "id", "ccurrent", "cmax"), &Entity::creceive_stat);

	ClassDB::bind_method(D_METHOD("sdie"), &Entity::sdie);
	ClassDB::bind_method(D_METHOD("cdie"), &Entity::cdie);

	ClassDB::bind_method(D_METHOD("ons_stat_changed", "stat"), &Entity::ons_stat_changed);
	ClassDB::bind_method(D_METHOD("onc_stat_changed", "stat"), &Entity::onc_stat_changed);

	//EventHandlers
	BIND_VMETHOD(MethodInfo("_son_before_cast", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	BIND_VMETHOD(MethodInfo("_son_before_cast_target", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	BIND_VMETHOD(MethodInfo("_son_cast_started", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	BIND_VMETHOD(MethodInfo("_son_cast_failed", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	BIND_VMETHOD(MethodInfo("_son_cast_finished", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	BIND_VMETHOD(MethodInfo("_son_cast_finished_target", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	BIND_VMETHOD(MethodInfo("_son_spell_cast_success", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));

	BIND_VMETHOD(MethodInfo("_son_before_damage_hit", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellDamageInfo")));
	BIND_VMETHOD(MethodInfo("_son_hit", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellDamageInfo")));

	BIND_VMETHOD(MethodInfo("_son_before_damage", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellDamageInfo")));
	BIND_VMETHOD(MethodInfo("_son_damage_receive", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellDamageInfo")));
	BIND_VMETHOD(MethodInfo("_son_dealt_damage", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellDamageInfo")));
	BIND_VMETHOD(MethodInfo("_son_damage_dealt", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellDamageInfo")));

	BIND_VMETHOD(MethodInfo("_son_before_heal_hit", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellHealInfo")));
	BIND_VMETHOD(MethodInfo("_son_before_heal", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellHealInfo")));
	BIND_VMETHOD(MethodInfo("_son_heal_receive", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellHealInfo")));
	BIND_VMETHOD(MethodInfo("_son_dealt_heal", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellHealInfo")));
	BIND_VMETHOD(MethodInfo("_son_heal_dealt", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellHealInfo")));

	BIND_VMETHOD(MethodInfo("_son_before_aura_applied", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "AuraData")));
	BIND_VMETHOD(MethodInfo("_son_after_aura_applied", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "AuraData")));

	BIND_VMETHOD(MethodInfo("_son_death"));

	BIND_VMETHOD(MethodInfo("_son_gcd_started", PropertyInfo(Variant::REAL, "gcd")));
	BIND_VMETHOD(MethodInfo("_son_gcd_finished"));

	BIND_VMETHOD(MethodInfo("_son_xp_gained", PropertyInfo(Variant::INT, "value")));
	BIND_VMETHOD(MethodInfo("_son_level_up", PropertyInfo(Variant::INT, "value")));

	BIND_VMETHOD(MethodInfo("_son_target_changed", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "old_target", PROPERTY_HINT_RESOURCE_TYPE, "Entity")));
	BIND_VMETHOD(MethodInfo("_con_target_changed", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "old_target", PROPERTY_HINT_RESOURCE_TYPE, "Entity")));

	ClassDB::bind_method(D_METHOD("son_before_aura_applied", "data"), &Entity::son_before_aura_applied);
	ClassDB::bind_method(D_METHOD("son_after_aura_applied", "data"), &Entity::son_after_aura_applied);

	ClassDB::bind_method(D_METHOD("son_before_damage_hit", "data"), &Entity::son_before_damage_hit);
	ClassDB::bind_method(D_METHOD("son_hit", "data"), &Entity::son_hit);
	ClassDB::bind_method(D_METHOD("son_before_damage", "data"), &Entity::son_before_damage);
	ClassDB::bind_method(D_METHOD("son_damage_receive", "data"), &Entity::son_damage_receive);
	ClassDB::bind_method(D_METHOD("son_dealt_damage", "data"), &Entity::son_dealt_damage);
	ClassDB::bind_method(D_METHOD("son_damage_dealt", "data"), &Entity::son_damage_dealt);

	ClassDB::bind_method(D_METHOD("son_before_heal_hit", "data"), &Entity::son_before_heal_hit);
	ClassDB::bind_method(D_METHOD("son_before_heal", "data"), &Entity::son_before_heal);
	ClassDB::bind_method(D_METHOD("son_heal_receive", "data"), &Entity::son_heal_receive);
	ClassDB::bind_method(D_METHOD("son_dealt_heal", "data"), &Entity::son_dealt_heal);
	ClassDB::bind_method(D_METHOD("son_heal_dealt", "data"), &Entity::son_heal_dealt);

	ClassDB::bind_method(D_METHOD("son_before_cast", "info"), &Entity::son_before_cast);
	ClassDB::bind_method(D_METHOD("son_before_cast_target", "info"), &Entity::son_before_cast_target);
	ClassDB::bind_method(D_METHOD("son_cast_finished_target", "info"), &Entity::son_cast_finished_target);
	ClassDB::bind_method(D_METHOD("son_cast_finished", "info"), &Entity::son_cast_finished);
	ClassDB::bind_method(D_METHOD("son_cast_started", "info"), &Entity::son_cast_started);
	ClassDB::bind_method(D_METHOD("son_cast_failed", "info"), &Entity::son_cast_failed);
	ClassDB::bind_method(D_METHOD("son_spell_cast_success", "info"), &Entity::son_spell_cast_success);

	ClassDB::bind_method(D_METHOD("son_death"), &Entity::son_death);

	ClassDB::bind_method(D_METHOD("son_gcd_started"), &Entity::son_gcd_started);
	ClassDB::bind_method(D_METHOD("son_gcd_finished"), &Entity::son_gcd_finished);

	//Talents

	ClassDB::bind_method(D_METHOD("gets_free_talent_points"), &Entity::gets_free_talent_points);
	ClassDB::bind_method(D_METHOD("sets_free_talent_points", "value"), &Entity::sets_free_talent_points);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "sfree_talent_points"), "sets_free_talent_points", "gets_free_talent_points");

	ClassDB::bind_method(D_METHOD("getc_free_talent_points"), &Entity::getc_free_talent_points);
	ClassDB::bind_method(D_METHOD("setc_free_talent_points", "value"), &Entity::setc_free_talent_points);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cfree_talent_points"), "setc_free_talent_points", "getc_free_talent_points");

	BIND_VMETHOD(MethodInfo("_sreceive_talent_learn_request", PropertyInfo(Variant::INT, "spec_index"), PropertyInfo(Variant::INT, "talent_row"), PropertyInfo(Variant::INT, "talent_culomn")));
	BIND_VMETHOD(MethodInfo("_sreceive_reset_talent_request"));

	ADD_SIGNAL(MethodInfo("stalent_learned", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::INT, "talent_id")));
	ADD_SIGNAL(MethodInfo("ctalent_learned", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::INT, "talent_id")));

	ADD_SIGNAL(MethodInfo("stalent_reset", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity")));
	ADD_SIGNAL(MethodInfo("ctalent_reset", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity")));

	BIND_VMETHOD(MethodInfo("_son_talent_learned", PropertyInfo(Variant::INT, "talent_id")));
	BIND_VMETHOD(MethodInfo("_con_talent_learned", PropertyInfo(Variant::INT, "talent_id")));

	BIND_VMETHOD(MethodInfo("_son_talent_reset"));
	BIND_VMETHOD(MethodInfo("_con_talent_reset"));

	ClassDB::bind_method(D_METHOD("crequest_talent_learn", "spec_index", "talent_row", "talent_culomn"), &Entity::crequest_talent_learn);
	ClassDB::bind_method(D_METHOD("sreceive_talent_learn_request", "spec_index", "talent_row", "talent_culomn"), &Entity::sreceive_talent_learn_request);
	ClassDB::bind_method(D_METHOD("_sreceive_talent_learn_request", "spec_index", "talent_row", "talent_culomn"), &Entity::_sreceive_talent_learn_request);

	ClassDB::bind_method(D_METHOD("crequest_talent_reset"), &Entity::crequest_talent_reset);
	ClassDB::bind_method(D_METHOD("sreceive_reset_talent_request"), &Entity::sreceive_reset_talent_request);
	ClassDB::bind_method(D_METHOD("_sreceive_reset_talent_request"), &Entity::_sreceive_reset_talent_request);

	ClassDB::bind_method(D_METHOD("sreset_talents"), &Entity::sreset_talents);
	ClassDB::bind_method(D_METHOD("creset_talents"), &Entity::creset_talents);

	ClassDB::bind_method(D_METHOD("adds_talent", "talent"), &Entity::adds_talent);
	ClassDB::bind_method(D_METHOD("removes_talent", "talent"), &Entity::removes_talent);
	ClassDB::bind_method(D_METHOD("hass_talent", "talent"), &Entity::hass_talent);
	ClassDB::bind_method(D_METHOD("gets_talent", "index"), &Entity::gets_talent);
	ClassDB::bind_method(D_METHOD("gets_talent_count"), &Entity::gets_talent_count);
	ClassDB::bind_method(D_METHOD("sclear_talents"), &Entity::sclear_talents);

	ClassDB::bind_method(D_METHOD("addc_talent", "talent"), &Entity::addc_talent);
	ClassDB::bind_method(D_METHOD("removec_talent", "talent"), &Entity::removec_talent);
	ClassDB::bind_method(D_METHOD("hasc_talent", "talent"), &Entity::hasc_talent);
	ClassDB::bind_method(D_METHOD("getc_talent", "index"), &Entity::getc_talent);
	ClassDB::bind_method(D_METHOD("getc_talent_count"), &Entity::getc_talent_count);
	ClassDB::bind_method(D_METHOD("cclear_talents"), &Entity::cclear_talents);

	//Clientside EventHandlers
	BIND_VMETHOD(MethodInfo("_con_cast_failed", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	BIND_VMETHOD(MethodInfo("_con_cast_started", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	BIND_VMETHOD(MethodInfo("_con_cast_state_changed", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	BIND_VMETHOD(MethodInfo("_con_cast_finished", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));
	BIND_VMETHOD(MethodInfo("_con_spell_cast_success", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo")));

	BIND_VMETHOD(MethodInfo("_con_death"));

	BIND_VMETHOD(MethodInfo("con_cooldown_added", PropertyInfo(Variant::OBJECT, "cooldown", PROPERTY_HINT_RESOURCE_TYPE, "Cooldown")));
	BIND_VMETHOD(MethodInfo("con_cooldown_removed", PropertyInfo(Variant::OBJECT, "cooldown", PROPERTY_HINT_RESOURCE_TYPE, "Cooldown")));
	BIND_VMETHOD(MethodInfo("con_category_cooldown_added", PropertyInfo(Variant::OBJECT, "category_cooldown", PROPERTY_HINT_RESOURCE_TYPE, "CategoryCooldown")));
	BIND_VMETHOD(MethodInfo("con_category_cooldown_removed", PropertyInfo(Variant::OBJECT, "category_cooldown", PROPERTY_HINT_RESOURCE_TYPE, "CategoryCooldown")));

	BIND_VMETHOD(MethodInfo("_con_aura_added", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "AuraData")));
	BIND_VMETHOD(MethodInfo("_con_aura_removed", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "AuraData")));
	BIND_VMETHOD(MethodInfo("_con_aura_refresh", PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "AuraData")));

	BIND_VMETHOD(MethodInfo("_con_damage_dealt", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellDamageInfo")));
	BIND_VMETHOD(MethodInfo("_con_dealt_damage", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellDamageInfo")));
	BIND_VMETHOD(MethodInfo("_con_heal_dealt", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellHealInfo")));
	BIND_VMETHOD(MethodInfo("_con_dealt_heal", PropertyInfo(Variant::OBJECT, "info", PROPERTY_HINT_RESOURCE_TYPE, "SpellHealInfo")));

	BIND_VMETHOD(MethodInfo("_con_gcd_started", PropertyInfo(Variant::REAL, "gcd")));
	BIND_VMETHOD(MethodInfo("_con_gcd_finished"));

	BIND_VMETHOD(MethodInfo("_con_xp_gained", PropertyInfo(Variant::INT, "value")));
	BIND_VMETHOD(MethodInfo("_con_level_up", PropertyInfo(Variant::INT, "value")));

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "value"), "_canc_interact"));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "value"), "_cans_interact"));
	BIND_VMETHOD(MethodInfo("_sinteract"));

	ClassDB::bind_method(D_METHOD("con_cast_failed", "info"), &Entity::con_cast_failed);
	ClassDB::bind_method(D_METHOD("con_cast_started", "info"), &Entity::con_cast_started);
	ClassDB::bind_method(D_METHOD("con_cast_state_changed", "info"), &Entity::con_cast_state_changed);
	ClassDB::bind_method(D_METHOD("con_cast_finished", "info"), &Entity::con_cast_finished);
	ClassDB::bind_method(D_METHOD("cspell_cast_success", "info"), &Entity::cspell_cast_success);
	ClassDB::bind_method(D_METHOD("cspell_cast_success_rpc", "data"), &Entity::cspell_cast_success_rpc);

	ClassDB::bind_method(D_METHOD("con_spell_cast_success", "info"), &Entity::con_spell_cast_success);

	ClassDB::bind_method(D_METHOD("con_death"), &Entity::con_death);

	ClassDB::bind_method(D_METHOD("con_cooldown_added", "cooldown"), &Entity::con_cooldown_added);
	ClassDB::bind_method(D_METHOD("con_cooldown_removed", "cooldown"), &Entity::con_cooldown_removed);
	ClassDB::bind_method(D_METHOD("con_category_cooldown_added", "category_cooldown"), &Entity::con_category_cooldown_added);
	ClassDB::bind_method(D_METHOD("con_category_cooldown_removed", "category_cooldown"), &Entity::con_category_cooldown_removed);

	ClassDB::bind_method(D_METHOD("con_aura_added", "data"), &Entity::con_aura_added);
	ClassDB::bind_method(D_METHOD("con_aura_removed", "data"), &Entity::con_aura_removed);
	ClassDB::bind_method(D_METHOD("con_aura_refresh", "data"), &Entity::con_aura_refresh);

	ClassDB::bind_method(D_METHOD("con_damage_dealt", "info"), &Entity::con_damage_dealt);
	ClassDB::bind_method(D_METHOD("con_dealt_damage", "info"), &Entity::con_dealt_damage);
	ClassDB::bind_method(D_METHOD("con_heal_dealt", "info"), &Entity::con_heal_dealt);
	ClassDB::bind_method(D_METHOD("con_dealt_heal", "info"), &Entity::con_dealt_heal);

	ClassDB::bind_method(D_METHOD("con_gcd_started"), &Entity::con_gcd_started);
	ClassDB::bind_method(D_METHOD("con_gcd_finished"), &Entity::con_gcd_finished);

	ClassDB::bind_method(D_METHOD("con_xp_gained", "value"), &Entity::con_xp_gained);
	ClassDB::bind_method(D_METHOD("con_level_up", "value"), &Entity::con_level_up);

	//Modifiers/Requesters
	ClassDB::bind_method(D_METHOD("sapply_passives_damage_receive", "data"), &Entity::sapply_passives_damage_receive);
	ClassDB::bind_method(D_METHOD("sapply_passives_damage_deal", "data"), &Entity::sapply_passives_damage_deal);

	//Spell operations
	ClassDB::bind_method(D_METHOD("scast_spell", "spell_id"), &Entity::scast_spell);
	ClassDB::bind_method(D_METHOD("crequest_spell_cast", "spell_id"), &Entity::crequest_spell_cast);

	//Damage Operations
	ClassDB::bind_method(D_METHOD("stake_damage", "data"), &Entity::stake_damage);
	ClassDB::bind_method(D_METHOD("sdeal_damage_to", "data"), &Entity::sdeal_damage_to);

	//Heal Operations
	ClassDB::bind_method(D_METHOD("stake_heal", "data"), &Entity::stake_heal);
	ClassDB::bind_method(D_METHOD("sdeal_heal_to", "data"), &Entity::sdeal_heal_to);

	//Damage, Heal RPCs
	ClassDB::bind_method(D_METHOD("cdamage_dealt_rpc", "data"), &Entity::cdamage_dealt_rpc);
	ClassDB::bind_method(D_METHOD("cdealt_damage_rpc", "data"), &Entity::cdealt_damage_rpc);
	ClassDB::bind_method(D_METHOD("cheal_dealt_rpc", "data"), &Entity::cheal_dealt_rpc);
	ClassDB::bind_method(D_METHOD("cdealt_heal_rpc", "data"), &Entity::cdealt_heal_rpc);

	//Interactions
	ClassDB::bind_method(D_METHOD("cans_interact"), &Entity::cans_interact);
	ClassDB::bind_method(D_METHOD("sinteract"), &Entity::sinteract);

	ClassDB::bind_method(D_METHOD("canc_interact"), &Entity::canc_interact);
	ClassDB::bind_method(D_METHOD("crequest_interact"), &Entity::crequest_interact);

	ClassDB::bind_method(D_METHOD("ssend_open_window", "window_id"), &Entity::ssend_open_window);
	ClassDB::bind_method(D_METHOD("copen_window", "window_id"), &Entity::copen_window);

	//XP Operations
	ADD_SIGNAL(MethodInfo("son_xp_gained", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::INT, "value")));
	ADD_SIGNAL(MethodInfo("con_xp_gained", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::INT, "value")));

	ADD_SIGNAL(MethodInfo("con_level_up", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::INT, "value")));
	ADD_SIGNAL(MethodInfo("son_level_up", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::INT, "value")));

	ADD_SIGNAL(MethodInfo("son_level_changed", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::INT, "level")));
	ADD_SIGNAL(MethodInfo("con_level_changed", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::INT, "level")));

	ClassDB::bind_method(D_METHOD("adds_xp", "value"), &Entity::adds_xp);
	ClassDB::bind_method(D_METHOD("addc_xp", "value"), &Entity::addc_xp);

	ClassDB::bind_method(D_METHOD("slevelup", "value"), &Entity::slevelup);
	ClassDB::bind_method(D_METHOD("clevelup", "value"), &Entity::clevelup);

	ClassDB::bind_method(D_METHOD("son_xp_gained", "value"), &Entity::son_xp_gained);
	ClassDB::bind_method(D_METHOD("son_level_up", "value"), &Entity::son_level_up);

	//Aura Manipulation
	ClassDB::bind_method(D_METHOD("sadd_aura", "aura"), &Entity::sadd_aura);
	ClassDB::bind_method(D_METHOD("sremove_aura", "aura"), &Entity::sremove_aura);
	ClassDB::bind_method(D_METHOD("sremove_aura_exact", "aura"), &Entity::sremove_aura_exact);
	ClassDB::bind_method(D_METHOD("sremove_aura_expired", "aura"), &Entity::sremove_aura_expired);
	ClassDB::bind_method(D_METHOD("sremove_aura_dispelled", "aura"), &Entity::sremove_aura_dispelled);
	//ClassDB::bind_method(D_METHOD("saura_refreshed", "aura"), &Entity::saura_refreshed);

	ClassDB::bind_method(D_METHOD("cadd_aura_rpc", "data"), &Entity::cadd_aura_rpc);
	ClassDB::bind_method(D_METHOD("cremove_aura_rpc", "data"), &Entity::cremove_aura_rpc);
	ClassDB::bind_method(D_METHOD("cremove_aura_exact_rpc", "data"), &Entity::cremove_aura_exact_rpc);
	ClassDB::bind_method(D_METHOD("cremove_aura_expired_rpc", "data"), &Entity::cremove_aura_expired_rpc);
	ClassDB::bind_method(D_METHOD("cremove_aura_dispelled_rpc", "data"), &Entity::cremove_aura_dispelled_rpc);

	ClassDB::bind_method(D_METHOD("cadd_aura", "aura"), &Entity::cadd_aura);
	ClassDB::bind_method(D_METHOD("cremove_aura", "aura"), &Entity::cremove_aura);
	ClassDB::bind_method(D_METHOD("cremove_aura_exact", "aura"), &Entity::cremove_aura_exact);
	ClassDB::bind_method(D_METHOD("cremove_aura_expired", "aura"), &Entity::cremove_aura_expired);
	ClassDB::bind_method(D_METHOD("cremove_aura_dispelled", "aura"), &Entity::cremove_aura_dispelled);
	//ClassDB::bind_method(D_METHOD("caura_refreshed", "aura"), &Entity::caura_refreshed);

	ClassDB::bind_method(D_METHOD("sremove_auras_with_group", "aura_group"), &Entity::sremove_auras_with_group);

	ClassDB::bind_method(D_METHOD("sget_aura_count"), &Entity::sget_aura_count);
	ClassDB::bind_method(D_METHOD("sget_aura", "index"), &Entity::sget_aura);
	ClassDB::bind_method(D_METHOD("sget_aura_by", "caster", "aura_id"), &Entity::sget_aura_by_bind);
	ClassDB::bind_method(D_METHOD("sget_aura_with_group_by", "caster", "aura_group"), &Entity::sget_aura_with_group_by_bind);

	ClassDB::bind_method(D_METHOD("cget_aura_count"), &Entity::cget_aura_count);
	ClassDB::bind_method(D_METHOD("cget_aura", "index"), &Entity::cget_aura);

	//Hooks
	BIND_VMETHOD(MethodInfo("_moved"));
	ClassDB::bind_method(D_METHOD("moved"), &Entity::moved);

	ADD_SIGNAL(MethodInfo("onc_mouse_entered"));
	ADD_SIGNAL(MethodInfo("onc_mouse_exited"));

	BIND_VMETHOD(MethodInfo("_onc_mouse_enter"));
	BIND_VMETHOD(MethodInfo("_onc_mouse_exit"));

	ClassDB::bind_method(D_METHOD("onc_mouse_enter"), &Entity::onc_mouse_enter);
	ClassDB::bind_method(D_METHOD("onc_mouse_exit"), &Entity::onc_mouse_exit);

	ADD_SIGNAL(MethodInfo("onc_targeted"));
	ADD_SIGNAL(MethodInfo("onc_untargeted"));

	BIND_VMETHOD(MethodInfo("_onc_targeted"));
	BIND_VMETHOD(MethodInfo("_onc_untargeted"));

	ClassDB::bind_method(D_METHOD("onc_targeted"), &Entity::onc_targeted);
	ClassDB::bind_method(D_METHOD("onc_untargeted"), &Entity::onc_untargeted);

	//Properties
	ClassDB::bind_method(D_METHOD("get_body_path"), &Entity::get_body_path);
	ClassDB::bind_method(D_METHOD("set_body_path", "value"), &Entity::set_body_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "body_path"), "set_body_path", "get_body_path");

	ClassDB::bind_method(D_METHOD("get_character_skeleton_path"), &Entity::get_character_skeleton_path);
	ClassDB::bind_method(D_METHOD("set_character_skeleton_path", "value"), &Entity::set_character_skeleton_path);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "character_skeleton_path"), "set_character_skeleton_path", "get_character_skeleton_path");

	ClassDB::bind_method(D_METHOD("gets_entity_data_id"), &Entity::gets_entity_data_id);
	ClassDB::bind_method(D_METHOD("sets_entity_data_id", "value"), &Entity::sets_entity_data_id);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "characterclass_id"), "sets_entity_data_id", "gets_entity_data_id");

	ClassDB::bind_method(D_METHOD("getc_entity_data_id"), &Entity::getc_entity_data_id);
	ClassDB::bind_method(D_METHOD("setc_entity_data_id", "value"), &Entity::setc_entity_data_id);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "entity_data_id"), "setc_entity_data_id", "getc_entity_data_id");

	ClassDB::bind_method(D_METHOD("gets_entity_type"), &Entity::gets_entity_type);
	ClassDB::bind_method(D_METHOD("sets_entity_type", "value"), &Entity::sets_entity_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "sentity_type", PROPERTY_HINT_ENUM, EntityEnums::BINDING_STRING_ENTITY_TYPES), "sets_entity_type", "gets_entity_type");

	ClassDB::bind_method(D_METHOD("getc_entity_type"), &Entity::getc_entity_type);
	ClassDB::bind_method(D_METHOD("setc_entity_type", "value"), &Entity::sets_entity_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "centity_type", PROPERTY_HINT_ENUM, EntityEnums::BINDING_STRING_ENTITY_TYPES), "setc_entity_type", "getc_entity_type");

	ClassDB::bind_method(D_METHOD("gets_ai_state"), &Entity::gets_ai_state);
	ClassDB::bind_method(D_METHOD("sets_ai_state", "value"), &Entity::sets_ai_state);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "ai_state", PROPERTY_HINT_ENUM, EntityEnums::BINDING_STRING_AI_STATES), "sets_ai_state", "gets_ai_state");

	ClassDB::bind_method(D_METHOD("gets_seed"), &Entity::gets_seed);
	ClassDB::bind_method(D_METHOD("sets_seed", "value"), &Entity::sets_seed);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "sseed"), "sets_seed", "gets_seed");

	ClassDB::bind_method(D_METHOD("getc_seed"), &Entity::getc_seed);
	ClassDB::bind_method(D_METHOD("setc_seed", "value"), &Entity::setc_seed);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cseed"), "setc_seed", "getc_seed");

	//Interaction type
	ClassDB::bind_method(D_METHOD("gets_entity_interaction_type"), &Entity::gets_entity_interaction_type);
	ClassDB::bind_method(D_METHOD("sets_entity_interaction_type", "value"), &Entity::sets_entity_interaction_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "sentity_interaction_type", PROPERTY_HINT_ENUM, EntityEnums::BINDING_STRING_ENTITY_INTERACTION_TYPE), "sets_entity_interaction_type", "gets_entity_interaction_type");

	ClassDB::bind_method(D_METHOD("getc_entity_interaction_type"), &Entity::getc_entity_interaction_type);
	ClassDB::bind_method(D_METHOD("setc_entity_interaction_type", "value"), &Entity::setc_entity_interaction_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "centity_interaction_type", PROPERTY_HINT_ENUM, EntityEnums::BINDING_STRING_ENTITY_INTERACTION_TYPE), "setc_entity_interaction_type", "getc_entity_interaction_type");

	ClassDB::bind_method(D_METHOD("gets_immunity_flags"), &Entity::gets_immunity_flags);
	ClassDB::bind_method(D_METHOD("sets_immunity_flags", "value"), &Entity::sets_immunity_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "simmunity_flags", PROPERTY_HINT_FLAGS, EntityEnums::BINDING_STRING_ENTITY_IMMUNITY_FLAGS), "sets_immunity_flags", "gets_immunity_flags");

	ClassDB::bind_method(D_METHOD("gets_entity_flags"), &Entity::gets_entity_flags);
	ClassDB::bind_method(D_METHOD("sets_entity_flags", "value"), &Entity::sets_entity_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "sentity_flags", PROPERTY_HINT_FLAGS, EntityEnums::BINDING_STRING_ENTITY_FLAGS), "sets_entity_flags", "gets_entity_flags");

	ClassDB::bind_method(D_METHOD("getc_entity_flags"), &Entity::getc_entity_flags);
	ClassDB::bind_method(D_METHOD("setc_entity_flags", "value"), &Entity::setc_entity_flags);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "centity_flags", PROPERTY_HINT_FLAGS, EntityEnums::BINDING_STRING_ENTITY_FLAGS), "setc_entity_flags", "getc_entity_flags");

	ClassDB::bind_method(D_METHOD("gets_entity_name"), &Entity::gets_entity_name);
	ClassDB::bind_method(D_METHOD("sets_entity_name", "value"), &Entity::sets_entity_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "sentity_name"), "sets_entity_name", "gets_entity_name");

	ClassDB::bind_method(D_METHOD("getc_entity_name"), &Entity::getc_entity_name);
	ClassDB::bind_method(D_METHOD("setc_entity_name", "value"), &Entity::setc_entity_name);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "centity_name"), "setc_entity_name", "getc_entity_name");

	ClassDB::bind_method(D_METHOD("gets_gender"), &Entity::gets_gender);
	ClassDB::bind_method(D_METHOD("sets_gender", "value"), &Entity::sets_gender);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "sgender"), "sets_gender", "gets_gender");

	ClassDB::bind_method(D_METHOD("getc_gender"), &Entity::getc_gender);
	ClassDB::bind_method(D_METHOD("setc_gender", "value"), &Entity::setc_gender);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cgender"), "setc_gender", "getc_gender");

	ClassDB::bind_method(D_METHOD("gets_level"), &Entity::gets_level);
	ClassDB::bind_method(D_METHOD("sets_level", "value"), &Entity::sets_level);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "slevel"), "sets_level", "gets_level");

	ClassDB::bind_method(D_METHOD("getc_level"), &Entity::getc_level);
	ClassDB::bind_method(D_METHOD("setc_level", "value"), &Entity::setc_level);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "clevel"), "setc_level", "getc_level");

	ClassDB::bind_method(D_METHOD("gets_xp"), &Entity::gets_xp);
	ClassDB::bind_method(D_METHOD("sets_xp", "value"), &Entity::sets_xp);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "sxp"), "sets_xp", "gets_xp");

	ClassDB::bind_method(D_METHOD("getc_xp"), &Entity::getc_xp);
	ClassDB::bind_method(D_METHOD("setc_xp", "value"), &Entity::setc_xp);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cxp"), "setc_xp", "getc_xp");

	ClassDB::bind_method(D_METHOD("gets_money"), &Entity::gets_money);
	ClassDB::bind_method(D_METHOD("sets_money", "value"), &Entity::sets_money);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "smoney"), "sets_money", "gets_money");

	ClassDB::bind_method(D_METHOD("getc_money"), &Entity::getc_money);
	ClassDB::bind_method(D_METHOD("setc_money", "value"), &Entity::setc_money);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cmoney"), "setc_money", "getc_money");

	ClassDB::bind_method(D_METHOD("gets_entity_data"), &Entity::gets_entity_data);
	ClassDB::bind_method(D_METHOD("sets_entity_data", "value"), &Entity::sets_entity_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "sentity_data", PROPERTY_HINT_RESOURCE_TYPE, "EntityData"), "sets_entity_data", "gets_entity_data");

	ClassDB::bind_method(D_METHOD("getc_entity_data"), &Entity::getc_entity_data);
	ClassDB::bind_method(D_METHOD("setc_entity_data", "value"), &Entity::setc_entity_data);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "centity_data", PROPERTY_HINT_RESOURCE_TYPE, "EntityData"), "setc_entity_data", "getc_entity_data");

	ClassDB::bind_method(D_METHOD("initialize", "entity_create_info"), &Entity::initialize);

	ClassDB::bind_method(D_METHOD("get_health"), &Entity::get_health);
	ClassDB::bind_method(D_METHOD("get_mana"), &Entity::get_mana);
	ClassDB::bind_method(D_METHOD("get_rage"), &Entity::get_rage);
	ClassDB::bind_method(D_METHOD("get_energy"), &Entity::get_energy);
	ClassDB::bind_method(D_METHOD("get_speed"), &Entity::get_speed);
	ClassDB::bind_method(D_METHOD("get_gcd"), &Entity::get_gcd);
	ClassDB::bind_method(D_METHOD("get_melee_crit"), &Entity::get_melee_crit);
	ClassDB::bind_method(D_METHOD("get_melee_crit_bonus"), &Entity::get_melee_crit_bonus);
	ClassDB::bind_method(D_METHOD("get_spell_crit"), &Entity::get_spell_crit);
	ClassDB::bind_method(D_METHOD("get_spell_crit_bonus"), &Entity::get_spell_crit_bonus);
	ClassDB::bind_method(D_METHOD("get_block"), &Entity::get_block);
	ClassDB::bind_method(D_METHOD("get_parry"), &Entity::get_parry);
	ClassDB::bind_method(D_METHOD("get_damage_reduction"), &Entity::get_damage_reduction);
	ClassDB::bind_method(D_METHOD("get_melee_damage_reduction"), &Entity::get_melee_damage_reduction);
	ClassDB::bind_method(D_METHOD("get_spell_damage_reduction"), &Entity::get_spell_damage_reduction);
	ClassDB::bind_method(D_METHOD("get_damage_taken"), &Entity::get_damage_taken);
	ClassDB::bind_method(D_METHOD("get_melee_damage"), &Entity::get_melee_damage);
	ClassDB::bind_method(D_METHOD("get_spell_damage"), &Entity::get_spell_damage);

	ClassDB::bind_method(D_METHOD("get_stat_int", "index"), &Entity::get_stat_int);
	ClassDB::bind_method(D_METHOD("set_stat_int", "index", "entry"), &Entity::set_stat_int);

	ClassDB::bind_method(D_METHOD("get_stat_enum", "index"), &Entity::get_stat_enum);
	ClassDB::bind_method(D_METHOD("set_stat_enum", "stat_id", "entry"), &Entity::set_stat_enum);

	//Equipment System

	ADD_SIGNAL(MethodInfo("son_equip_success", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::INT, "equip_slot"), PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance"), PropertyInfo(Variant::OBJECT, "old_item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance"), PropertyInfo(Variant::INT, "bag_slot")));
	ADD_SIGNAL(MethodInfo("son_equip_fail", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::INT, "equip_slot"), PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance"), PropertyInfo(Variant::OBJECT, "old_item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance"), PropertyInfo(Variant::INT, "bag_slot")));
	ADD_SIGNAL(MethodInfo("con_equip_success", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::INT, "equip_slot"), PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance"), PropertyInfo(Variant::OBJECT, "old_item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance"), PropertyInfo(Variant::INT, "bag_slot")));
	ADD_SIGNAL(MethodInfo("con_equip_fail", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::INT, "equip_slot"), PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance"), PropertyInfo(Variant::OBJECT, "old_item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance"), PropertyInfo(Variant::INT, "bag_slot")));

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "ret"), "_should_deny_equip", PropertyInfo(Variant::INT, "equip_slot"), PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance")));

	BIND_VMETHOD(MethodInfo("_son_equip_success", PropertyInfo(Variant::INT, "equip_slot"), PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance"), PropertyInfo(Variant::OBJECT, "old_item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance"), PropertyInfo(Variant::INT, "bag_slot")));
	BIND_VMETHOD(MethodInfo("_son_equip_fail", PropertyInfo(Variant::INT, "equip_slot"), PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance"), PropertyInfo(Variant::OBJECT, "old_item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance"), PropertyInfo(Variant::INT, "bag_slot")));
	BIND_VMETHOD(MethodInfo("_con_equip_success", PropertyInfo(Variant::INT, "equip_slot"), PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance"), PropertyInfo(Variant::OBJECT, "old_item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance"), PropertyInfo(Variant::INT, "bag_slot")));
	BIND_VMETHOD(MethodInfo("_con_equip_fail", PropertyInfo(Variant::INT, "equip_slot"), PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance"), PropertyInfo(Variant::OBJECT, "old_item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance"), PropertyInfo(Variant::INT, "bag_slot")));

	ADD_SIGNAL(MethodInfo("equipment_changed", PropertyInfo(Variant::INT, "slot")));

	ClassDB::bind_method(D_METHOD("should_deny_equip", "equip_slot", "item"), &Entity::should_deny_equip);

	ClassDB::bind_method(D_METHOD("son_equip_success", "equip_slot", "item", "old_item", "bag_slot"), &Entity::son_equip_success);
	ClassDB::bind_method(D_METHOD("son_equip_fail", "equip_slot", "item", "old_item", "bag_slot"), &Entity::son_equip_fail);
	ClassDB::bind_method(D_METHOD("con_equip_success", "equip_slot", "item", "old_item", "bag_slot"), &Entity::con_equip_success);
	ClassDB::bind_method(D_METHOD("con_equip_fail", "equip_slot", "item", "old_item", "bag_slot"), &Entity::con_equip_fail);

	BIND_VMETHOD(MethodInfo("_sequip", PropertyInfo(Variant::INT, "equip_slot"), PropertyInfo(Variant::INT, "bag_slot")));

	ClassDB::bind_method(D_METHOD("crequest_equip", "equip_slot", "bag_slot"), &Entity::crequest_equip);
	ClassDB::bind_method(D_METHOD("sequip", "equip_slot", "bag_slot"), &Entity::sequip);
	ClassDB::bind_method(D_METHOD("_sequip", "equip_slot", "bag_slot"), &Entity::_sequip);

	ClassDB::bind_method(D_METHOD("cequip_success", "equip_slot", "bag_slot"), &Entity::cequip_success);
	ClassDB::bind_method(D_METHOD("cequip_fail", "equip_slot", "bag_slot"), &Entity::cequip_fail);

	ClassDB::bind_method(D_METHOD("gets_equip_slot", "index"), &Entity::gets_equip_slot);
	ClassDB::bind_method(D_METHOD("getc_equip_slot", "index"), &Entity::getc_equip_slot);

	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::BOOL, "can"), "_can_equip_item", PropertyInfo(Variant::INT, "equip_slot", PROPERTY_HINT_ENUM, ItemEnums::BINDING_STRING_EQUIP_SLOTS), PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance")));

	ClassDB::bind_method(D_METHOD("can_equip_item", "equip_slot", "item"), &Entity::can_equip_item);
	ClassDB::bind_method(D_METHOD("_can_equip_item", "equip_slot", "item"), &Entity::_can_equip_item);

	BIND_VMETHOD(MethodInfo("_sapply_item", PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance")));
	BIND_VMETHOD(MethodInfo("_sdeapply_item", PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance")));

	ClassDB::bind_method(D_METHOD("sapply_item", "item"), &Entity::sapply_item);
	ClassDB::bind_method(D_METHOD("sdeapply_item", "item"), &Entity::sdeapply_item);

	ClassDB::bind_method(D_METHOD("_sapply_item", "item"), &Entity::_sapply_item);
	ClassDB::bind_method(D_METHOD("_sdeapply_item", "item"), &Entity::_sdeapply_item);

	BIND_VMETHOD(MethodInfo("_capply_item", PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance")));
	BIND_VMETHOD(MethodInfo("_cdeapply_item", PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance")));

	ClassDB::bind_method(D_METHOD("csapply_item", "item"), &Entity::capply_item);
	ClassDB::bind_method(D_METHOD("cdeapply_item", "item"), &Entity::cdeapply_item);

	ClassDB::bind_method(D_METHOD("_capply_item", "item"), &Entity::_capply_item);
	ClassDB::bind_method(D_METHOD("_cdeapply_item", "item"), &Entity::_cdeapply_item);

	//Resources
	ClassDB::bind_method(D_METHOD("gets_resource_index", "index"), &Entity::gets_resource_index);
	ClassDB::bind_method(D_METHOD("gets_resource_id", "type"), &Entity::gets_resource_id);
	ClassDB::bind_method(D_METHOD("adds_resource", "palyer_resource"), &Entity::adds_resource);
	ClassDB::bind_method(D_METHOD("gets_resource_count"), &Entity::gets_resource_count);
	ClassDB::bind_method(D_METHOD("removes_resource", "index"), &Entity::removes_resource);
	ClassDB::bind_method(D_METHOD("clears_resource"), &Entity::clears_resource);

	ClassDB::bind_method(D_METHOD("addc_resource_rpc", "index", "data"), &Entity::addc_resource_rpc);

	ClassDB::bind_method(D_METHOD("getc_resource_index", "index"), &Entity::getc_resource_index);
	ClassDB::bind_method(D_METHOD("getc_resource_id", "type"), &Entity::getc_resource_id);
	ClassDB::bind_method(D_METHOD("addc_resource", "palyer_resource"), &Entity::addc_resource);
	ClassDB::bind_method(D_METHOD("getc_resource_count"), &Entity::getc_resource_count);
	ClassDB::bind_method(D_METHOD("removec_resource", "index"), &Entity::removec_resource);
	ClassDB::bind_method(D_METHOD("clearc_resource"), &Entity::clearc_resource);

	ClassDB::bind_method(D_METHOD("sends_resource_current", "index", "current"), &Entity::sends_resource_current);
	ClassDB::bind_method(D_METHOD("sends_resource_curr_max", "index", "current", "max"), &Entity::sends_resource_curr_max);
	ClassDB::bind_method(D_METHOD("sends_resource_data", "index", "data"), &Entity::sends_resource_data);

	ClassDB::bind_method(D_METHOD("creceive_resource_current", "index", "current"), &Entity::creceive_resource_current);
	ClassDB::bind_method(D_METHOD("creceive_resource_curr_max", "index", "current", "max"), &Entity::creceive_resource_curr_max);
	ClassDB::bind_method(D_METHOD("creceive_resource_data", "index", "data"), &Entity::creceive_resource_data);

	//GCD
	ADD_SIGNAL(MethodInfo("sgcd_started", PropertyInfo(Variant::REAL, "value")));
	ADD_SIGNAL(MethodInfo("sgcd_finished"));
	ADD_SIGNAL(MethodInfo("cgcd_started", PropertyInfo(Variant::REAL, "value")));
	ADD_SIGNAL(MethodInfo("cgcd_finished"));

	ClassDB::bind_method(D_METHOD("getc_has_global_cooldown"), &Entity::getc_has_global_cooldown);
	ClassDB::bind_method(D_METHOD("gets_has_global_cooldown"), &Entity::gets_has_global_cooldown);
	ClassDB::bind_method(D_METHOD("getc_global_cooldown"), &Entity::getc_global_cooldown);
	ClassDB::bind_method(D_METHOD("gets_global_cooldown"), &Entity::gets_global_cooldown);
	ClassDB::bind_method(D_METHOD("sstart_global_cooldown", "value"), &Entity::sstart_global_cooldown);
	ClassDB::bind_method(D_METHOD("cstart_global_cooldown", "value"), &Entity::cstart_global_cooldown);

	//Data
	ClassDB::bind_method(D_METHOD("adds_data", "data"), &Entity::adds_data);
	ClassDB::bind_method(D_METHOD("removes_data", "index"), &Entity::removes_data);
	ClassDB::bind_method(D_METHOD("gets_data", "index"), &Entity::gets_data);
	ClassDB::bind_method(D_METHOD("gets_data_count"), &Entity::gets_data_count);

	ClassDB::bind_method(D_METHOD("addc_data", "data"), &Entity::addc_data);
	ClassDB::bind_method(D_METHOD("removec_data", "index"), &Entity::removec_data);
	ClassDB::bind_method(D_METHOD("getc_data", "index"), &Entity::getc_data);
	ClassDB::bind_method(D_METHOD("getc_data_count"), &Entity::getc_data_count);

	//States
	ADD_SIGNAL(MethodInfo("sstate_changed", PropertyInfo(Variant::INT, "value")));
	ADD_SIGNAL(MethodInfo("cstate_changed", PropertyInfo(Variant::INT, "value")));

	ClassDB::bind_method(D_METHOD("getc_state"), &Entity::getc_state);
	ClassDB::bind_method(D_METHOD("setc_state", "state"), &Entity::setc_state);
	ClassDB::bind_method(D_METHOD("gets_state"), &Entity::gets_state);
	ClassDB::bind_method(D_METHOD("sets_state", "state"), &Entity::sets_state);
	ClassDB::bind_method(D_METHOD("sadd_state_ref", "state_index"), &Entity::sadd_state_ref);
	ClassDB::bind_method(D_METHOD("sremove_state_ref", "state_index"), &Entity::sremove_state_ref);

	//Casting System

	ClassDB::bind_method(D_METHOD("sis_casting"), &Entity::sis_casting);
	ClassDB::bind_method(D_METHOD("cis_casting"), &Entity::cis_casting);

	ClassDB::bind_method(D_METHOD("gets_spell_cast_info"), &Entity::gets_spell_cast_info);
	ClassDB::bind_method(D_METHOD("sets_spell_cast_info", "value"), &Entity::sets_spell_cast_info);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "sspell_cast_info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo"), "sets_spell_cast_info", "gets_spell_cast_info");

	ClassDB::bind_method(D_METHOD("getc_spell_cast_info"), &Entity::getc_spell_cast_info);
	ClassDB::bind_method(D_METHOD("setc_spell_cast_info", "value"), &Entity::setc_spell_cast_info);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "cspell_cast_info", PROPERTY_HINT_RESOURCE_TYPE, "SpellCastInfo"), "setc_spell_cast_info", "getc_spell_cast_info");

	ClassDB::bind_method(D_METHOD("sstart_casting", "info"), &Entity::sstart_casting);
	ClassDB::bind_method(D_METHOD("sfail_cast"), &Entity::sfail_cast);
	ClassDB::bind_method(D_METHOD("sdelay_cast"), &Entity::sdelay_cast);
	ClassDB::bind_method(D_METHOD("sfinish_cast"), &Entity::sfinish_cast);
	ClassDB::bind_method(D_METHOD("sinterrupt_cast"), &Entity::sinterrupt_cast);

	ClassDB::bind_method(D_METHOD("cstart_casting_rpc", "data"), &Entity::cstart_casting_rpc);
	ClassDB::bind_method(D_METHOD("cstart_casting", "info"), &Entity::cstart_casting);
	ClassDB::bind_method(D_METHOD("cfail_cast"), &Entity::cfail_cast);
	ClassDB::bind_method(D_METHOD("cdelay_cast"), &Entity::cdelay_cast);
	ClassDB::bind_method(D_METHOD("cfinish_cast"), &Entity::cfinish_cast);
	ClassDB::bind_method(D_METHOD("cinterrupt_cast"), &Entity::cinterrupt_cast);
	ClassDB::bind_method(D_METHOD("sspell_cast_success", "info"), &Entity::sspell_cast_success);

	//Cooldowns
	ADD_SIGNAL(MethodInfo("scooldown_added", PropertyInfo(Variant::OBJECT, "cooldown", PROPERTY_HINT_RESOURCE_TYPE, "Cooldown")));
	ADD_SIGNAL(MethodInfo("scooldown_removed", PropertyInfo(Variant::OBJECT, "cooldown", PROPERTY_HINT_RESOURCE_TYPE, "Cooldown")));
	ADD_SIGNAL(MethodInfo("ccooldown_added", PropertyInfo(Variant::OBJECT, "cooldown", PROPERTY_HINT_RESOURCE_TYPE, "Cooldown")));
	ADD_SIGNAL(MethodInfo("ccooldown_removed", PropertyInfo(Variant::OBJECT, "cooldown", PROPERTY_HINT_RESOURCE_TYPE, "Cooldown")));

	ClassDB::bind_method(D_METHOD("hass_cooldown", "spell_id"), &Entity::hass_cooldown);
	ClassDB::bind_method(D_METHOD("adds_cooldown", "spell_id", "value"), &Entity::adds_cooldown);
	ClassDB::bind_method(D_METHOD("removes_cooldown", "spell_id"), &Entity::removes_cooldown);
	ClassDB::bind_method(D_METHOD("gets_cooldown", "spell_id"), &Entity::gets_cooldown);
	ClassDB::bind_method(D_METHOD("gets_cooldown_index", "index"), &Entity::gets_cooldown_index);
	ClassDB::bind_method(D_METHOD("gets_cooldown_count"), &Entity::gets_cooldown_count);

	ClassDB::bind_method(D_METHOD("hasc_cooldown", "spell_id"), &Entity::hasc_cooldown);
	ClassDB::bind_method(D_METHOD("addc_cooldown", "spell_id", "value"), &Entity::addc_cooldown);
	ClassDB::bind_method(D_METHOD("removec_cooldown", "spell_id"), &Entity::removec_cooldown);
	ClassDB::bind_method(D_METHOD("getc_cooldown", "spell_id"), &Entity::getc_cooldown);
	ClassDB::bind_method(D_METHOD("getc_cooldown_index", "index"), &Entity::getc_cooldown_index);
	ClassDB::bind_method(D_METHOD("getc_cooldown_count"), &Entity::getc_cooldown_count);

	//Category Cooldowns
	ADD_SIGNAL(MethodInfo("scategory_cooldown_added", PropertyInfo(Variant::OBJECT, "cooldown", PROPERTY_HINT_RESOURCE_TYPE, "CategoryCooldown")));
	ADD_SIGNAL(MethodInfo("scategory_cooldown_removed", PropertyInfo(Variant::OBJECT, "cooldown", PROPERTY_HINT_RESOURCE_TYPE, "CategoryCooldown")));
	ADD_SIGNAL(MethodInfo("ccategory_cooldown_added", PropertyInfo(Variant::OBJECT, "cooldown", PROPERTY_HINT_RESOURCE_TYPE, "CategoryCooldown")));
	ADD_SIGNAL(MethodInfo("ccategory_cooldown_removed", PropertyInfo(Variant::OBJECT, "cooldown", PROPERTY_HINT_RESOURCE_TYPE, "CategoryCooldown")));

	ClassDB::bind_method(D_METHOD("hass_category_cooldown", "category_id"), &Entity::hass_category_cooldown);
	ClassDB::bind_method(D_METHOD("adds_category_cooldown", "category_id", "value"), &Entity::adds_category_cooldown);
	ClassDB::bind_method(D_METHOD("removes_category_cooldown", "category_id"), &Entity::removes_category_cooldown);
	ClassDB::bind_method(D_METHOD("gets_category_cooldown", "category_id"), &Entity::gets_category_cooldown);
	ClassDB::bind_method(D_METHOD("gets_category_cooldown_index", "index"), &Entity::gets_category_cooldown_index);
	ClassDB::bind_method(D_METHOD("gets_category_cooldown_count"), &Entity::gets_category_cooldown_count);

	ClassDB::bind_method(D_METHOD("hasc_category_cooldown", "category_id"), &Entity::hasc_category_cooldown);
	ClassDB::bind_method(D_METHOD("addc_category_cooldown", "category_id", "value"), &Entity::addc_category_cooldown);
	ClassDB::bind_method(D_METHOD("removec_category_cooldown", "category_id"), &Entity::removec_category_cooldown);
	ClassDB::bind_method(D_METHOD("getc_category_cooldown", "category_id"), &Entity::getc_category_cooldown);
	ClassDB::bind_method(D_METHOD("getc_category_cooldown_index", "index"), &Entity::getc_category_cooldown_index);
	ClassDB::bind_method(D_METHOD("getc_category_cooldown_count"), &Entity::getc_category_cooldown_count);

	//Known Spells
	ADD_SIGNAL(MethodInfo("sspell_added", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "spell", PROPERTY_HINT_RESOURCE_TYPE, "Spell")));
	ADD_SIGNAL(MethodInfo("sspell_removed", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "spell", PROPERTY_HINT_RESOURCE_TYPE, "Spell")));
	ADD_SIGNAL(MethodInfo("cspell_added", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "spell", PROPERTY_HINT_RESOURCE_TYPE, "Spell")));
	ADD_SIGNAL(MethodInfo("cspell_removed", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "spell", PROPERTY_HINT_RESOURCE_TYPE, "Spell")));

	ADD_SIGNAL(MethodInfo("sfree_spell_points_changed", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::INT, "new_value")));
	ADD_SIGNAL(MethodInfo("cfree_spell_points_changed", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::INT, "new_value")));

	ClassDB::bind_method(D_METHOD("gets_free_spell_points"), &Entity::gets_free_spell_points);
	ClassDB::bind_method(D_METHOD("sets_free_spell_points", "value"), &Entity::sets_free_spell_points);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "sfree_spell_points"), "sets_free_spell_points", "gets_free_spell_points");

	ClassDB::bind_method(D_METHOD("getc_free_spell_points"), &Entity::getc_free_spell_points);
	ClassDB::bind_method(D_METHOD("setc_free_spell_points", "value"), &Entity::setc_free_spell_points);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cfree_spell_points"), "setc_free_spell_points", "getc_free_spell_points");

	ClassDB::bind_method(D_METHOD("crequest_spell_learn", "id"), &Entity::crequest_spell_learn);
	ClassDB::bind_method(D_METHOD("slearn_spell", "id"), &Entity::slearn_spell);

	ClassDB::bind_method(D_METHOD("hass_spell", "spell"), &Entity::hass_spell);
	ClassDB::bind_method(D_METHOD("hass_spell_id", "id"), &Entity::hass_spell_id);
	ClassDB::bind_method(D_METHOD("adds_spell", "spell"), &Entity::adds_spell);
	ClassDB::bind_method(D_METHOD("removes_spell", "spell"), &Entity::removes_spell);
	ClassDB::bind_method(D_METHOD("gets_spell", "spell"), &Entity::gets_spell);
	ClassDB::bind_method(D_METHOD("gets_spell_count"), &Entity::gets_spell_count);

	ClassDB::bind_method(D_METHOD("hasc_spell", "spell"), &Entity::hasc_spell);
	ClassDB::bind_method(D_METHOD("hasc_spell_id", "id"), &Entity::hasc_spell_id);
	ClassDB::bind_method(D_METHOD("addc_spell", "spell"), &Entity::addc_spell);
	ClassDB::bind_method(D_METHOD("removec_spell", "spell"), &Entity::removec_spell);
	ClassDB::bind_method(D_METHOD("getc_spell", "spell"), &Entity::getc_spell);
	ClassDB::bind_method(D_METHOD("getc_spell_count"), &Entity::getc_spell_count);

	ClassDB::bind_method(D_METHOD("addc_spell_rpc", "id"), &Entity::addc_spell_rpc);
	ClassDB::bind_method(D_METHOD("removec_spell_rpc", "id"), &Entity::removec_spell_rpc);

	//Crafting
	BIND_VMETHOD(MethodInfo("_scraft", PropertyInfo(Variant::INT, "id")));

	ADD_SIGNAL(MethodInfo("scraft_success", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance")));
	ADD_SIGNAL(MethodInfo("ccraft_success", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "ItemInstance")));

	ADD_SIGNAL(MethodInfo("scraft_recipe_added", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "craft_recipe", PROPERTY_HINT_RESOURCE_TYPE, "CraftRecipe")));
	ADD_SIGNAL(MethodInfo("ccraft_recipe_added", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "craft_recipe", PROPERTY_HINT_RESOURCE_TYPE, "CraftRecipe")));

	ClassDB::bind_method(D_METHOD("crequest_craft", "id"), &Entity::crequest_craft);
	ClassDB::bind_method(D_METHOD("scraft", "id"), &Entity::scraft);

	ClassDB::bind_method(D_METHOD("hass_craft_recipe", "craft_recipe"), &Entity::hass_craft_recipe);
	ClassDB::bind_method(D_METHOD("hass_craft_recipe_id", "id"), &Entity::hass_craft_recipe_id);
	ClassDB::bind_method(D_METHOD("adds_craft_recipe", "craft_recipe"), &Entity::adds_craft_recipe);
	ClassDB::bind_method(D_METHOD("adds_craft_recipe_id", "id"), &Entity::adds_craft_recipe_id);
	ClassDB::bind_method(D_METHOD("removes_craft_recipe", "craft_recipe"), &Entity::removes_craft_recipe);
	ClassDB::bind_method(D_METHOD("removes_craft_recipe_id", "id"), &Entity::removes_craft_recipe_id);
	ClassDB::bind_method(D_METHOD("gets_craft_recipe", "index"), &Entity::gets_craft_recipe);
	ClassDB::bind_method(D_METHOD("gets_craft_recipe_id", "id"), &Entity::gets_craft_recipe_id);
	ClassDB::bind_method(D_METHOD("gets_craft_recipe_count"), &Entity::gets_craft_recipe_count);

	ClassDB::bind_method(D_METHOD("hasc_craft_recipe", "craft_recipe"), &Entity::hasc_craft_recipe);
	ClassDB::bind_method(D_METHOD("hasc_craft_recipe_id", "id"), &Entity::hasc_craft_recipe_id);
	ClassDB::bind_method(D_METHOD("addc_craft_recipe", "craft_recipe"), &Entity::addc_craft_recipe);
	ClassDB::bind_method(D_METHOD("addc_craft_recipe_id", "id"), &Entity::addc_craft_recipe_id);
	ClassDB::bind_method(D_METHOD("removec_craft_recipe", "craft_recipe"), &Entity::removec_craft_recipe);
	ClassDB::bind_method(D_METHOD("removec_craft_recipe_id", "id"), &Entity::removec_craft_recipe_id);
	ClassDB::bind_method(D_METHOD("getc_craft_recipe", "craft_recipe"), &Entity::getc_craft_recipe);
	ClassDB::bind_method(D_METHOD("getc_craft_recipe_count"), &Entity::getc_craft_recipe_count);

	//Skills
	ClassDB::bind_method(D_METHOD("hass_skill_id", "id"), &Entity::hass_skill_id);
	ClassDB::bind_method(D_METHOD("hass_skill", "skill"), &Entity::hass_skill);
	ClassDB::bind_method(D_METHOD("adds_skill", "skill"), &Entity::adds_skill);
	ClassDB::bind_method(D_METHOD("removes_skill", "skill"), &Entity::removes_skill);
	ClassDB::bind_method(D_METHOD("gets_skill", "skill"), &Entity::gets_skill);
	ClassDB::bind_method(D_METHOD("gets_skill_count"), &Entity::gets_skill_count);

	ClassDB::bind_method(D_METHOD("hasc_skill_id", "id"), &Entity::hasc_skill_id);
	ClassDB::bind_method(D_METHOD("hasc_skill", "skill"), &Entity::hasc_skill);
	ClassDB::bind_method(D_METHOD("addc_skill", "skill"), &Entity::addc_skill);
	ClassDB::bind_method(D_METHOD("removec_skill", "skill"), &Entity::removec_skill);
	ClassDB::bind_method(D_METHOD("getc_skill", "skill"), &Entity::getc_skill);
	ClassDB::bind_method(D_METHOD("getc_skill_count"), &Entity::getc_skill_count);

	ClassDB::bind_method(D_METHOD("addc_skill_id", "skill_id", "value", "max_value"), &Entity::addc_skill_id);
	ClassDB::bind_method(D_METHOD("removec_skill_id", "skill_id"), &Entity::removec_skill_id);
	ClassDB::bind_method(D_METHOD("changec_skill", "skill_id", "value"), &Entity::changec_skill);
	ClassDB::bind_method(D_METHOD("changec_skill_max", "skill_id", "value"), &Entity::changec_skill_max);

	//skeleton
	ClassDB::bind_method(D_METHOD("get_body"), &Entity::get_body);
	ClassDB::bind_method(D_METHOD("get_character_skeleton"), &Entity::get_character_skeleton);

	////    Targeting System    ////

	ClassDB::bind_method(D_METHOD("crequest_target_change", "path"), &Entity::crequest_target_change);
	ClassDB::bind_method(D_METHOD("net_sets_target", "path"), &Entity::net_sets_target);
	ClassDB::bind_method(D_METHOD("net_setc_target", "path"), &Entity::net_setc_target);

	ClassDB::bind_method(D_METHOD("gets_target"), &Entity::gets_target);
	ClassDB::bind_method(D_METHOD("sets_target", "target"), &Entity::sets_target);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "starget", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), "sets_target", "gets_target");

	ClassDB::bind_method(D_METHOD("getc_target"), &Entity::getc_target);
	ClassDB::bind_method(D_METHOD("setc_target", "target"), &Entity::setc_target);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "ctarget", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), "setc_target", "getc_target");

	////    Inventory System    ////

	ADD_SIGNAL(MethodInfo("sbag_changed", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "bag", PROPERTY_HINT_RESOURCE_TYPE, "Bag")));
	ADD_SIGNAL(MethodInfo("cbag_changed", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "bag", PROPERTY_HINT_RESOURCE_TYPE, "Bag")));

	ClassDB::bind_method(D_METHOD("gets_bag"), &Entity::gets_bag);
	ClassDB::bind_method(D_METHOD("sets_bag", "bag"), &Entity::sets_bag);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "sbag", PROPERTY_HINT_RESOURCE_TYPE, "Bag"), "sets_bag", "gets_bag");

	ClassDB::bind_method(D_METHOD("getc_bag"), &Entity::getc_bag);
	ClassDB::bind_method(D_METHOD("setc_bag", "bag"), &Entity::setc_bag);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "cbag", PROPERTY_HINT_RESOURCE_TYPE, "Bag"), "setc_bag", "getc_bag");

	ADD_SIGNAL(MethodInfo("starget_bag_changed", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "bag", PROPERTY_HINT_RESOURCE_TYPE, "Bag")));
	ADD_SIGNAL(MethodInfo("ctarget_bag_changed", PropertyInfo(Variant::OBJECT, "entity", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), PropertyInfo(Variant::OBJECT, "bag", PROPERTY_HINT_RESOURCE_TYPE, "Bag")));

	ClassDB::bind_method(D_METHOD("gets_target_bag"), &Entity::gets_target_bag);
	ClassDB::bind_method(D_METHOD("sets_target_bag", "bag"), &Entity::sets_target_bag);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "starget_bag", PROPERTY_HINT_RESOURCE_TYPE, "Bag"), "sets_target_bag", "gets_target_bag");

	ClassDB::bind_method(D_METHOD("getc_target_bag"), &Entity::getc_target_bag);
	ClassDB::bind_method(D_METHOD("setc_target_bag", "bag"), &Entity::setc_target_bag);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "ctarget_bag", PROPERTY_HINT_RESOURCE_TYPE, "Bag"), "setc_target_bag", "getc_target_bag");

	ClassDB::bind_method(D_METHOD("setc_bag_rpc", "data"), &Entity::setc_bag_rpc);
	ClassDB::bind_method(D_METHOD("setc_target_bag_rpc", "data"), &Entity::setc_target_bag_rpc);

	ClassDB::bind_method(D_METHOD("crequest_loot"), &Entity::crequest_loot);
	ClassDB::bind_method(D_METHOD("sloot"), &Entity::sloot);
	ClassDB::bind_method(D_METHOD("cloot"), &Entity::cloot);

	//Bag
	ClassDB::bind_method(D_METHOD("ons_item_added", "bag", "item", "slot_id"), &Entity::ons_item_added);
	ClassDB::bind_method(D_METHOD("cadd_item_rpc", "slot_id", "item_data"), &Entity::cadd_item_rpc);
	ClassDB::bind_method(D_METHOD("cadd_item", "slot_id", "item"), &Entity::cadd_item);

	ClassDB::bind_method(D_METHOD("ons_item_removed", "bag", "item", "slot_id"), &Entity::ons_item_removed);
	ClassDB::bind_method(D_METHOD("sremove_item", "slot_id"), &Entity::sremove_item);
	ClassDB::bind_method(D_METHOD("cremove_item", "slot_id"), &Entity::cremove_item);
	ClassDB::bind_method(D_METHOD("cdenyremove_item", "slot_id"), &Entity::cdenyremove_item);
	ClassDB::bind_method(D_METHOD("crequest_remove_item", "slot_id"), &Entity::crequest_remove_item);

	ClassDB::bind_method(D_METHOD("ons_items_swapped", "bag", "slot_id_1", "slot_id_2"), &Entity::ons_items_swapped);
	ClassDB::bind_method(D_METHOD("sswap_items", "slot_id_1", "slot_id_2"), &Entity::sswap_items);
	ClassDB::bind_method(D_METHOD("cswap_items", "slot_id_1", "slot_id_2"), &Entity::cswap_items);
	ClassDB::bind_method(D_METHOD("cdeny_item_swap", "slot_id_1", "slot_id_2"), &Entity::cdeny_item_swap);
	ClassDB::bind_method(D_METHOD("crequest_item_swap", "slot_id_1", "slot_id_2"), &Entity::crequest_item_swap);

	ClassDB::bind_method(D_METHOD("ons_item_count_changed", "bag", "item", "slot_id"), &Entity::ons_item_count_changed);
	ClassDB::bind_method(D_METHOD("cchange_item_count", "slot_id", "new_count"), &Entity::cchange_item_count);

	ClassDB::bind_method(D_METHOD("ons_overburdened", "bag"), &Entity::ons_overburdened);
	ClassDB::bind_method(D_METHOD("ons_overburden_removed", "bag"), &Entity::ons_overburden_removed);

	//target Bag
	ClassDB::bind_method(D_METHOD("ons_target_item_added", "bag", "item", "slot_id"), &Entity::ons_target_item_added);
	ClassDB::bind_method(D_METHOD("cadd_target_item_rpc", "slot_id", "item_data"), &Entity::cadd_target_item_rpc);
	ClassDB::bind_method(D_METHOD("cadd_target_item", "slot_id", "item"), &Entity::cadd_target_item);

	ClassDB::bind_method(D_METHOD("ons_target_item_removed", "bag", "item", "slot_id"), &Entity::ons_target_item_removed);
	ClassDB::bind_method(D_METHOD("sremove_target_item", "slot_id"), &Entity::sremove_target_item);
	ClassDB::bind_method(D_METHOD("cremove_target_item", "slot_id"), &Entity::cremove_target_item);
	ClassDB::bind_method(D_METHOD("cdenyremove_target_item", "slot_id"), &Entity::cdenyremove_target_item);
	ClassDB::bind_method(D_METHOD("crequest_target_remove_item", "slot_id"), &Entity::crequest_target_remove_item);

	ClassDB::bind_method(D_METHOD("ons_target_items_swapped", "bag", "slot_id_1", "slot_id_2"), &Entity::ons_target_items_swapped);
	ClassDB::bind_method(D_METHOD("sswap_target_items", "slot_id_1", "slot_id_2"), &Entity::sswap_target_items);
	ClassDB::bind_method(D_METHOD("cswap_target_items", "slot_id_1", "slot_id_2"), &Entity::cswap_target_items);
	ClassDB::bind_method(D_METHOD("cdeny_target_item_swap", "slot_id_1", "slot_id_2"), &Entity::cdeny_target_item_swap);
	ClassDB::bind_method(D_METHOD("crequest_target_item_swap", "slot_id_1", "slot_id_2"), &Entity::crequest_target_item_swap);

	ClassDB::bind_method(D_METHOD("ons_target_item_count_changed", "bag", "item", "slot_id"), &Entity::ons_target_item_count_changed);
	ClassDB::bind_method(D_METHOD("cchange_target_item_count", "slot_id", "new_count"), &Entity::cchange_target_item_count);

	//Actionbars

	ClassDB::bind_method(D_METHOD("get_actionbar_locked"), &Entity::get_actionbar_locked);
	ClassDB::bind_method(D_METHOD("set_actionbar_locked", "value"), &Entity::set_actionbar_locked);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "actionbar_locked"), "set_actionbar_locked", "get_actionbar_locked");

	ClassDB::bind_method(D_METHOD("get_action_bar_profile"), &Entity::get_action_bar_profile);

	//Pets

	ClassDB::bind_method(D_METHOD("adds_pet", "entity"), &Entity::adds_pet_bind);
	ClassDB::bind_method(D_METHOD("gets_pet", "index"), &Entity::gets_pet);
	ClassDB::bind_method(D_METHOD("removes_pet_index", "index"), &Entity::removes_pet_index);
	ClassDB::bind_method(D_METHOD("removes_pet", "entity"), &Entity::removes_pet_bind);
	ClassDB::bind_method(D_METHOD("gets_pet_count"), &Entity::gets_pet_count);

	ClassDB::bind_method(D_METHOD("addc_pet_path"), &Entity::addc_pet_path);
	
	ClassDB::bind_method(D_METHOD("addc_pet", "entity"), &Entity::addc_pet_bind);
	ClassDB::bind_method(D_METHOD("getc_pet", "index"), &Entity::getc_pet);
	ClassDB::bind_method(D_METHOD("removec_pet_index", "index"), &Entity::removec_pet_index);
	ClassDB::bind_method(D_METHOD("removec_pet", "entity"), &Entity::removec_pet_bind);
	ClassDB::bind_method(D_METHOD("getc_pet_count"), &Entity::getc_pet_count);

	//ClassDB::bind_method(D_METHOD("pets_attack"), &Entity::pets_attack);
	//ClassDB::bind_method(D_METHOD("pets_follow"), &Entity::pets_follow);
	//ClassDB::bind_method(D_METHOD("pets_stop"), &Entity::pets_stop);

	// AI

	ClassDB::bind_method(D_METHOD("gets_is_pet"), &Entity::gets_is_pet);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "sis_pet"), "", "gets_is_pet");

	ClassDB::bind_method(D_METHOD("getc_is_pet"), &Entity::getc_is_pet);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cis_pet"), "", "getc_is_pet");

	ClassDB::bind_method(D_METHOD("gets_pet_owner"), &Entity::gets_pet_owner);
	ClassDB::bind_method(D_METHOD("sets_pet_owner", "entity"), &Entity::sets_pet_owner_bind);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "spet_owner", PROPERTY_HINT_RESOURCE_TYPE, "Entity"), "sets_pet_owner", "gets_pet_owner");

	ClassDB::bind_method(D_METHOD("gets_pet_formation_index"), &Entity::gets_pet_formation_index);
	ClassDB::bind_method(D_METHOD("sets_pet_formation_index", "value"), &Entity::sets_pet_formation_index);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "spet_formation_index"), "sets_pet_formation_index", "gets_pet_formation_index");

	ClassDB::bind_method(D_METHOD("gets_pet_ai_state"), &Entity::gets_pet_ai_state);
	ClassDB::bind_method(D_METHOD("sets_pet_ai_state", "value"), &Entity::sets_pet_ai_state);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "gets_pet_ai_state", PROPERTY_HINT_ENUM, EntityEnums::BINDING_STRING_AI_STATES), "sets_pet_ai_state", "gets_pet_ai_state");

	ClassDB::bind_method(D_METHOD("gets_original_entity_controller"), &Entity::gets_original_entity_controller);
	ClassDB::bind_method(D_METHOD("sets_original_entity_controller", "value"), &Entity::sets_original_entity_controller);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "soriginal_entity_controller", PROPERTY_HINT_ENUM, EntityEnums::BINDING_STRING_ENTITY_CONTOLLER), "sets_original_entity_controller", "gets_original_entity_controller");

	ClassDB::bind_method(D_METHOD("gets_entity_controller"), &Entity::gets_entity_controller);
	ClassDB::bind_method(D_METHOD("sets_entity_controller", "value"), &Entity::sets_entity_controller);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "sentity_controller", PROPERTY_HINT_ENUM, EntityEnums::BINDING_STRING_ENTITY_CONTOLLER), "sets_entity_controller", "gets_entity_controller");

	ClassDB::bind_method(D_METHOD("gets_ai"), &Entity::gets_ai);
	ClassDB::bind_method(D_METHOD("sets_ai", "value"), &Entity::sets_ai);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "sai", PROPERTY_HINT_RESOURCE_TYPE, "EntityAI"), "sets_ai", "gets_ai");

	//Serialization
	BIND_VMETHOD(MethodInfo("_from_dict", PropertyInfo(Variant::DICTIONARY, "dict")));
	BIND_VMETHOD(MethodInfo(PropertyInfo(Variant::DICTIONARY, "dict"), "_to_dict"));

	ClassDB::bind_method(D_METHOD("is_deserialized"), &Entity::is_deserialized);

	ClassDB::bind_method(D_METHOD("from_dict", "dict"), &Entity::from_dict);
	ClassDB::bind_method(D_METHOD("to_dict"), &Entity::to_dict);

	ClassDB::bind_method(D_METHOD("_from_dict", "dict"), &Entity::_from_dict);
	ClassDB::bind_method(D_METHOD("_to_dict"), &Entity::_to_dict);

	//Networking
	ClassDB::bind_method(D_METHOD("gets_sees", "index"), &Entity::gets_sees);
	ClassDB::bind_method(D_METHOD("removes_sees_index", "index"), &Entity::removes_sees_index);
	ClassDB::bind_method(D_METHOD("removes_sees", "entity"), &Entity::removes_sees_bind);
	ClassDB::bind_method(D_METHOD("adds_sees", "entity"), &Entity::adds_sees_bind);
	ClassDB::bind_method(D_METHOD("gets_sees_count"), &Entity::gets_sees_count);

	ClassDB::bind_method(D_METHOD("gets_seen_by", "index"), &Entity::gets_seen_by);
	ClassDB::bind_method(D_METHOD("removes_seen_by_index", "index"), &Entity::removes_seen_by_index);
	ClassDB::bind_method(D_METHOD("removes_seen_by", "entity"), &Entity::removes_seen_by_bind);
	ClassDB::bind_method(D_METHOD("adds_seen_by", "entity"), &Entity::adds_seen_by_bind);
	ClassDB::bind_method(D_METHOD("gets_seen_by_count"), &Entity::gets_seen_by_count);

	MethodInfo mi;

	mi.arguments.push_back(PropertyInfo(Variant::STRING, "method"));

	mi.name = "vrpc";
	ClassDB::bind_vararg_method(METHOD_FLAGS_DEFAULT, "vrpc", &Entity::_vrpc_bind, mi);

	ClassDB::bind_method(D_METHOD("register_for_physics_process", "info"), &Entity::register_for_physics_process);

	ClassDB::bind_method(D_METHOD("get_maunal_process"), &Entity::get_maunal_process);
	ClassDB::bind_method(D_METHOD("set_maunal_process", "value"), &Entity::set_maunal_process);

	ClassDB::bind_method(D_METHOD("update", "delta"), &Entity::update);
}
