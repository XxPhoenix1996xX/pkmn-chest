#include "langStrings.hpp"

namespace Lang {
	std::vector<std::string>
	// In game text strings
	abilities, games, items, locations4, locations5, moves, natures, species,
	// App strings
	aMenuText = {"Move", "Edit", "Copy", "Release", "Dump", "Back"},
	aMenuEmptySlotText = {"Inject", "Create", "Back"},
	aMenuTopBarText = {"Jump to box", "Rename", "Swap", "Wallpaper", "Dump box", "Back"},
	chestFileMenuText = {"New", "Rename", "Delete", "Change"},
	filterLabels = {"Species", "Nature", "Ability", "Gender", "Item", "Ball", "Form", "Level", "Moves", "Shiny"},
	optionsTextLabels = {"Chest File", "Chest Size", "Language", "Backups", "Music", "Sound Effects", "D-Pad typing directions", "D-Pad typing groups"},
	originLabels = {"Met level", "Met year", "Met month", "Met day", "Met location", "Origin game"},
	pouch = {"Items", "Key Items", "TMs & HMs", "Mail", "Medicine", "Berries", "Poké Balls", "Battle Items"},
	songs = {"Off", "Pokémon Center (Gen. 1)", "Pokémon Center (Gen. 4)", "Pokémon Center (Gen. 5)", "Twinleaf Town", "Prof. Elm's Lab", "Prof. Oak's Lab", "Game Corner"},
	summaryLabels = {"Species", "Name", "Level", "Ability", "Nature", "Item", "Shiny", "Pokérus", "Orig. Trainer", "Trainer ID", "Secret ID", "Friendship"},
	statsLabels = {"HP.", "Attack", "Defense", "Sp. Atk", "Sp. Def", "Speed", "Base", "IV", "EV", "Total"},
	trainerText = {"Name", "Trainer ID", "Secret ID", "Money", "BP", "Badges", "Play time"},
	xMenuText = {"Party", "Options", "Bag", "", "Save", "Exit"};

	std::string
	// [main]
	cancel = "Cancel",
	chest = "Chest",
	dexNo = "No.",
	discard = "Discard",
	female = "Female",
	filter = "Filter",
	hpType = "Hidden Power type:",
	invalidSave = "Invalid save file",
	level = "Lv.",
	loading = "Loading...",
	male = "Male",
	movesString = "Moves",
	no = "False",
	origin = "Origin",
	stats = "Stats",
	release = "Release",
	remove = "Remove",
	save = "Save",
	unlimited = "Unlimited",
	yes = "True",
	// [saveMsg]
	saveMsgCard = "Would you like to save changes\nto the gamecard?",
	saveMsgChest = "Would you like to save changes\nto the chest?",
	saveMsgSave = "Would you like to save changes\nto the save file?";
}
