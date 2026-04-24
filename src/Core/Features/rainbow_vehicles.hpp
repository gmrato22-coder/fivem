////////////////Discord///////////////
///https://discord.gg/skylineengine///
//////////////////////////////////////
namespace features {
    void rainbow_vehicles()
    {
        int hue = 0;

        while (true)
        {
            Sleep(20);

            if (!globals::toggles::rainbow_vehicle_every)
                continue;

            std::lock_guard<std::mutex> lock_vehicle(globals::mutex::vehicle_list_mutex);

            float h = hue / 60.0f;
            int r, g, b;

            int i = static_cast<int>(h);
            float f = h - i;
            float p = 0.0f;
            float q = 1.0f - f;
            float t = f;

            switch (i) {
            case 0:
                r = 255;
                g = static_cast<int>(t * 255);
                b = 0;
                break;
            case 1:
                r = static_cast<int>(q * 255);
                g = 255;
                b = 0;
                break;
            case 2:
                r = 0;
                g = 255;
                b = static_cast<int>(t * 255);
                break;
            case 3:
                r = 0;
                g = static_cast<int>(q * 255);
                b = 255;
                break;
            case 4:
                r = static_cast<int>(t * 255);
                g = 0;
                b = 255;
                break;
            case 5:
                r = 255;
                g = 0;
                b = static_cast<int>(q * 255);
                break;
            default:
                r = g = b = 0;
                break;
            }


            for (const auto& vehicle : vehicle_list::vehicle_list)
            {
                network* net = network::instance();

                net->network_request_control_of_entity(vehicle.ptr, game::local_player);

                sdk::c_vehicle::c_vehicle_draw_handler::c_custom_shader_effect_vehicle::set_primary_vehicle_color(vehicle.ptr, r, g, b);
                sdk::c_vehicle::c_vehicle_draw_handler::c_custom_shader_effect_vehicle::set_secondary_vehicle_color(vehicle.ptr, r, g, b);


            }
            hue = (hue + 1) % 360;


        }
    }
}
