#pragma once

#include <array>

struct G502
{
	static constexpr unsigned short VID = 0x046D;
	static constexpr unsigned short PID = 0xC539;
	static constexpr unsigned short USAGE_PAGE = 0xFF00;
	static constexpr unsigned short USAGE = 2;

	static constexpr std::array<int, 100> voltages =
	{
		4186, 4156, 4143, 4133, 4122, 4113, 4103, 4094, 4086, 4075,
		4067, 4059, 4051, 4043, 4035, 4027, 4019, 4011, 4003, 3997,
		3989, 3983, 3976, 3969, 3961, 3955, 3949, 3942, 3935, 3929,
		3922, 3916, 3909, 3902, 3896, 3890, 3883, 3877, 3870, 3865,
		3859, 3853, 3848, 3842, 3837, 3833, 3828, 3824, 3819, 3815,
		3811, 3808, 3804, 3800, 3797, 3793, 3790, 3787, 3784, 3781,
		3778, 3775, 3772, 3770, 3767, 3764, 3762, 3759, 3757, 3754,
		3751, 3748, 3744, 3741, 3737, 3734, 3730, 3726, 3724, 3720,
		3717, 3714, 3710, 3706, 3702, 3697, 3693, 3688, 3683, 3677,
		3671, 3666, 3662, 3658, 3654, 3646, 3633, 3612, 3579, 3537
	};

	static constexpr unsigned short getPercentFromVoltage(unsigned short voltage)
	{
		if (voltage > 4200 || voltage < 3500)
			return 0;

		size_t idx = 0;

		for (; idx < voltages.size(); ++idx)
		{
			if (voltages[idx] < voltage)
				break;
		}

		return (voltages.size() - idx);
	}
};

