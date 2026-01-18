#include "LibMain.h"

#include <cstdint>

using GPUtils = gigperformer::sdk::GPUtils;

/// Ignore a given value
/// \details this is a dummy function to suppress compiler warnings about unused parameters
template <typename T> void Ignore(T const &) noexcept
{
}

// define an XML string describing your product
const std::string XMLProductDescription =
    // Replace with your information
    "<Library>"
    "  <Product"
    "    Name=\"Rackspace Songlist\""
    "    Version=\"1.0\""
    "    BuildDate=\"01/18/2026\">"
    "  </Product> "
    "  <Description>Produces a list of Songs that use each Rackspace</Description>"
    "  <ImagePath></ImagePath>"
    "</Library>";

// List of panels
std::vector<std::string> panelNames = {
};
std::vector<std::string> relativePanelLocations = {
};

std::string pathToMe; // This needs to be initialized from the initialization
                      // section of the LibMain class so it can be used in the
                      // standalone functions directly below

int LibMain::GetPanelCount()
{
    return static_cast<int>(panelNames.size());
}

std::string LibMain::GetPanelName(int index)
{
    std::string text;
    if (index >= 0 && static_cast<std::size_t>(index) < panelNames.size())
    {
        text = panelNames[index];
    }

    return text;
}

// Return panel layout in XML format
std::string LibMain::GetPanelXML(int index)
{
    std::string text;
    if (index >= 0 && static_cast<std::size_t>(index) < panelNames.size())
    {
        // We assume the panels are in the same folder as the library
        GPUtils::loadTextFile(getPathToMe() + "/" + relativePanelLocations[index], text);
    }
    return text;
}

// List of menu items
std::vector<std::string> menuNames = {
    "Create Rackspace-Songlist",
};

int LibMain::GetMenuCount()
{
    return static_cast<int>(menuNames.size());
}

std::string LibMain::GetMenuName(int index)
{
    std::string text;
    if (index >= 0 && static_cast<std::size_t>(index) < menuNames.size())
    {
        text = menuNames[index];
    }

    return text;
}

void LibMain::InvokeMenu(int index)
{
    if (index >= 0 && static_cast<std::size_t>(index) < menuNames.size())
    {
        switch (index)
        {
        case 0:
            MakeRackspaceSongList();
            break;
        case 1:
            switchToWiringView();
            break;
        case 2:
            switchToSetlistView();
            break;

        default:
            break;
        }
    }
}


void LibMain::Initialization()
{
    registerCallback("OnSongChanged");
    registerCallback("OnSongPartChanged");
    registerCallback("OnRackspaceActivated");
}

std::string LibMain::GetProductDescription()
{
    // Generally don't touch this - simply define the constant
    // 'XMLProductDescription' at the top of this file with an XML description of
    // your product
    return XMLProductDescription;
}


void LibMain::OnRackspaceActivated()
{
    // scriptLog("Rackspace activated: " + getRackspaceName(getCurrentRackspaceIndex()), 1);
    currentrackspace = getCurrentRackspaceIndex();
}

void LibMain::OnSongChanged(int oldIndex, int newIndex)
{
    currentsong = newIndex;
}

void LibMain::OnSongPartChanged(int oldIndex, int newIndex)
{
    if (runningracksonglist)
    {
        // scriptLog("Songpart changed from " + std::to_string(oldIndex) + " to " + std::to_string(newIndex), 1);
        std::string songvarname =
            getSongName(getCurrentSongIndex()) + "[" + getSongpartName(getCurrentSongIndex(), newIndex) + "]";
        racksonglist[static_cast<std::size_t>(currentrackspace)].push_back(songvarname);
        if (getCurrentSongIndex() == getSongCount() - 1 &&
            getCurrentSongpartIndex() == getSongpartCount(getCurrentSongIndex()) - 1)
        {
            ShowRackspaceSongList();
            runningracksonglist = false;
        }
    }
}

void LibMain::ShowRackspaceSongList()
    // Output per rack: reset outputline for each rack to avoid accumulation.
{
    std::string outputline = "";

    for (int rack = 0; rack < getRackspaceCount(); ++rack)
    {
        outputline = std::to_string(rack + 1) + " - ";
        const auto &row = racksonglist[static_cast<std::size_t>(rack)];
        if (!row.empty())
        {
            // First element is the rack name (added above).
            outputline += row[0];
            if (row.size() > 1)
            {
                outputline += ": ";
            }
            else
            {
                outputline += ": <no songs>";
            }

            for (std::size_t i = 1; i < row.size(); ++i)
            {
                if (i > 1)
                {
                    outputline += ", ";
                }
                outputline += row[i];
            }
        }
        scriptLog(outputline, 1);
    }
}

void LibMain::MakeRackspaceSongList()
{
    int rackCount = getRackspaceCount();
    bool mode = inSetlistMode();
    std::string outputline = "";

    if (rackCount <= 0)
    {
        return;
    }

    // Ensure we have one vector per rackspace to avoid out-of-range access.
    racksonglist.resize(static_cast<std::size_t>(rackCount));

    // Make rack name the first element of each row.
    for (int r = 0; r < rackCount; ++r)
    {
        racksonglist[static_cast<std::size_t>(r)].clear();
        racksonglist[static_cast<std::size_t>(r)].push_back(getRackspaceName(r));
    }

    switchToSetlistView();
    switchToSong(0, 0);

    int songCount = getSongCount();

    if (songCount <= 0)
    {
        return;
    }

    scriptLog("Cycle through all song parts to build rackspace-songlist...", 1);
    runningracksonglist = true;
    scriptLog("Rackspace count: " + std::to_string(rackCount) + "   Songcount: " + std::to_string(songCount), 1);


    /*
    for (int song = 0; song < songCount; song++)
    {
        switchToSong(song, 0);
        int partCount = getSongpartCount(song);
        for (int songpart = 0; songpart < partCount; songpart++)
        {
            switchToSongPart(songpart);

            int currentRack = getCurrentRackspaceIndex();
            if (currentRack < 0 || currentRack >= rackCount)
            {
                // Skip entries that map to invalid rack indices.
                continue;
            }

            std::string songvarname = getSongName(song) + "[" + getSongpartName(song, songpart) + "]";
            racksonglist[static_cast<std::size_t>(currentRack)].push_back(songvarname);
        }
    }

    // unregisterCallback("OnVariationChanged");

    if (!mode)
    {
        switchToPanelView();
    } */
}       

namespace gigperformer
{
namespace sdk
{

GigPerformerAPI *CreateGPExtension(LibraryHandle handle)
{
    return new LibMain(handle);
}

} // namespace sdk
} // namespace gigperformer
