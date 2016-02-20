/*   KSSed - A level editor for Kirby Super Star/Kirby's Fun Pak and Kirby of the Stars Super Deluxe
 *   Copyright (C) 2015  Morten Delenk
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "../kssed.hpp"
ConfigurationManager* config = nullptr;
ConfigurationManager::ConfigurationManager() {
  config=this;
  video.append(video.driver, "Driver");
  append(video, "Video");
  load(locate({configpath(), "kssed/"}, "settings.bml"));
  if(!video.driver) video.driver = ruby::Video::safestDriver();
  save(locate({configpath(), "kssed/"}, "settings.bml"));
}
auto ConfigurationManager::quit() ->void {
  save(locate({configpath(),"kssed/"},"settings.bml"));
}
