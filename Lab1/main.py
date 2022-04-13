DATA_LIST = [315, 245, 11, 290, 47, 49, 595, 618, 559,
				530, 369, 1035, 546, 673, 11, 314, 1156, 33,
				1201, 1055, 241, 91, 6, 1596, 680, 1100, 97,
				307, 771, 2063, 46, 25, 42, 112, 124, 245,
				19, 214, 719, 38, 278, 285, 719, 274, 110,
				543, 72, 176, 650, 2105, 546, 1088, 187, 36,
				213, 148, 813, 933, 857, 1512, 669, 307, 41,
				190, 99, 215, 260, 363, 388, 532, 1486,
				1445, 516, 351, 497, 67, 1200, 142, 50, 629,
				252, 410, 772, 973, 13, 410, 1401, 1656,
				721, 75, 243, 1318, 535, 629, 266, 487, 349,
				38, 95, 1533]
DATA_y = 0.98
DATA_T1 = 1098
DATA_T2 = 2055

print("Середній наробіток до відмови Tср = {:.2f}".format(sum(DATA_LIST)/len(DATA_LIST)))

DATA_LIST.sort()
print("Відсортована вибірка:\n", DATA_LIST)
print("\nМаксимальним значенням наробітку до відмови є", DATA_LIST[-1])

ten_div = DATA_LIST[-1]/10
print("\nПоділимо інтервал від 0 до максимального наробітку до відмови на 10 частин (інтервалів), тоді довжина одного інтервалу буде дорівнювати", 
	ten_div, 
	", а границі інтервалів будуть наступними:\n", 
	*["{}й інтервал від {:.1f} до {:.1f}\n".format(i+1, i*ten_div, (i+1)*ten_div) for i in range(10)])

def get_f_arr(arr, div):
	ret = []
	for i in range(10):
		ret.append([i, []])
		for j in arr:
			if j >= i * div and j <= (i + 1) * div:
				ret[-1][1].append(j)
	return ret

print("\nЗначення статистичної щільності розподілу ймовірності відмови для кожного інтервалу:\n", 
	*["для {}-го інтервалу f = {:.10f}\n".format(i+1, len(j)/100/ten_div) for i,j in get_f_arr(DATA_LIST, ten_div)])

def get_p_arr(arr, div):
	arr = [(i, len(j)/100) for i,j in get_f_arr(arr, div)]
	ret = []
	pi = 0
	for i,j in arr:
		pi += j
		ret.append((i, 1-pi))
	return ret

print("\nЗначення ймовірності безвідмовної роботи пристрою на час правої границі інтервалу для кожного інтервалу:\n\n P(0) = 1.00\n\n", 
	*["для {}-го інтервалу P({:.1f}) = {:.2f}\n".format(i+1, (i+1)*ten_div, abs(j)) for i,j in get_p_arr(DATA_LIST, ten_div)])

def get_interval(arr, t_y):
	for i,j in [(-1, 1.0)] + arr:
		if j >= t_y > arr[i+1][1]:
			return i+1, i+2, arr[i+1][1], j

intervals_array = get_interval(get_p_arr(DATA_LIST, ten_div), DATA_y)
d = (intervals_array[2] - DATA_y) / (intervals_array[2] - intervals_array[3])
print("d({0:},{1:}) = ({2:.2f} - {4:.2f}) / ({2:.2f} - {3:.2f}) = {5:}".format(*intervals_array, DATA_y, d))
print("T({0:.2f}) = {1:.1f} - {1:.1f} * {2:} = {3:}".format(DATA_y, ten_div, d, ten_div - ten_div * d))

def f_sentence(arr, div, time):
	formula = "1 - ("
	for i,j in get_p_arr(arr, div):
		formula += "f{}*{:.1f} + ".format(i+1, div)
		if time < (i+2) * div:
			return formula + "f{}*({} - {:.1f}))".format(i+2, time, (i+1) * div), \
				j - len(get_f_arr(arr, div)[i+1][1])/100/div*(time - (i+1) * div), i+1, len(get_f_arr(arr, div)[i+1][1])/100/div

print("\nЙмовірність безвідмовної роботи на час {0:} годин:\nP({0:}) = {1:} = {2:}".format(DATA_T1, *f_sentence(DATA_LIST, ten_div, DATA_T1)[:2]))

p_t2_arr = f_sentence(DATA_LIST, ten_div, DATA_T2)
print("\nЙмовірність відмов на час {0:} годин:\nP({0:}) = {1:} = {2:}".format(DATA_T2, *p_t2_arr[:2]))
print("λ({0:})= f{1:}/P({0:}) = {2:}".format(DATA_T2, p_t2_arr[2]+1, p_t2_arr[3]/p_t2_arr[1]))
