const error = require('../../../Response/Error');
const success = require('../../../Response/Success');

module.exports = {
	name:'now',
	category:'Service',
	description:'$now (jelenleg játszott szám cím  megjelíntése)',
	isload: true,
	run: async (bot, msg, args, guildqueue, user) => {
		if (args.length === 0) {
			if (guildqueue !== undefined && guildqueue !== null) {
				success.run(msg, 0, guildqueue, args, user);
			}
			else {
				error.run(msg, 4, guildqueue, args);
			}
		}
		else {
			error.run(msg, 1, guildqueue, args);
		}
	},
};