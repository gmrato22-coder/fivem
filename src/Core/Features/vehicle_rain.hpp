namespace features {
    void vehicle_rain()
    {
        while (true)
        {
            Sleep(1);

            std::lock_guard<std::mutex> lock(globals::mutex::entity_list_mutex);
            std::lock_guard<std::mutex> lock_vehicle(globals::mutex::vehicle_list_mutex);

            for (int i = 0; i < 2048; i++)
            {
                if (!globals::vehicle_rain[i] && !globals::toggles::vehicle_rain_every)
                    continue;

                int index = entity_list::get_index_by_net_id(i);
                int list_index = entity_list::get_list_index_by_net_id(i);
                Vector3 player_position = entity_list::ped_list[list_index].origin;

                for (const auto& vehicle : vehicle_list::vehicle_list)
                {
                    network* net = network::instance();

                    net->network_request_control_of_entity(vehicle.ptr, game::local_player);

                    sdk::c_vehicle::set_position(vehicle.ptr, player_position);
                }


            }
        }
    }
}
